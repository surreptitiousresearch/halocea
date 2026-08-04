// 0x82ADAB70 — sslSCRIPTED_FUNCTION_BASE::ExecCode
// The SSL script VM bytecode interpreter: a fetch/decode/execute loop over this function's
// compiled `bytecode` vector, dispatching on the DB-verified sslBC_CMD opcode set (40 ops).
// Returns sslERROR by value (sret).
//
// ============================ CAVEAT (semantic reconstruction notes) ============================
// 1. r3/r4 swap. The decompiler mistyped the hidden sret pointer (r3) as `this` and the real
//    `this` (r4) as `result`. Ground truth (from the demangled protected signature): r3 is the
//    by-value sslERROR return buffer, r4 is the sslSCRIPTED_FUNCTION_BASE*. This reconstruction
//    uses the corrected mapping: the method body operates on real `this`; every value it "returns"
//    is the sret sslERROR. Decompiler writes of `this->pDecl / pClassOfDefinition / stateFunc`
//    (the sslFUNCTION_BASE field names at off 0/4/8) are really field-by-field copies of an
//    sslERROR {id, desc.pBuffer, tags} into the sret buffer — i.e. plain `return <error>;`.
// 2. Decompiler `result[N]` array indexing (sizeof(sslERROR)==12) maps onto real members:
//    result[1]=consts, result[2].tags=usedTypes, result[4].desc=bytecode, result[4].tags=
//    bytecode.nElem, result[6]=rContainerObject, result->id=pDecl(as leading dsSTRID name),
//    result->desc.pBuffer=pClassOfDefinition. (DB types_members sslSCRIPTED_FUNCTION_BASE.)
// 3. Inlined destructor teardown. At every early return the compiler inlined the destructors of
//    the live local sslERROR / dsTSTRING / dsDATA temporaries (ref-count decrements + dlFree, and
//    dsDATA runtime-type Destroy). Those hundreds of duplicated teardown blocks are represented
//    here by ordinary C++ automatic destruction (RAII) of the scoped locals, which is what the
//    source expressed before the compiler expanded it.
// 4. Temp coalescing. The decompiler folded dozens of unrelated stack temporaries into fields of
//    one giant sslSYSTEM stack blob. They are unrelated locals; they are reconstructed here
//    as properly-scoped per-opcode locals with descriptive names.
// 5. Error channels. `err` holds the per-instruction result; `firstErr` records the FIRST soft
//    (recoverable) error. Fatal ops return the freshly built error immediately; a handful of ops
//    (name resolution, foreach-init) accumulate into `firstErr`, push a null/false placeholder,
//    and keep executing. Normal completion returns `firstErr`.
// 6. The 6th mangled parameter is dsDATA* (`callerData`); the body never references it.
// 7. The SSL_BC_LINE_START debugger path reads several fields through pClassOfDefinition's source
//    object whose sub-layout the decompiler typed unreliably; those accesses are best-effort and
//    marked inline.
// ===============================================================================================

#include "sslSCRIPTED_FUNCTION_BASE.h"
#include "sslBYTECODE_INSTRUCTION.h"
#include "sslSTACK.h"
#include "sslSTACK_DATA.h"
#include "sslERROR.h"
#include "sslCLASS.h"
#include "sslCLASS_REF.h"
#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "sslSYSTEM.h"
#include "sslFUNCTION_DECL.h"
#include "sslFOREACH_DATA.h"
#include "sslSOURCE.h"
#include "sslSOURCE_REF.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsDATA_MATH.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../ds/dsERROR_HANDLER.h"
#include "../con_/CONSOLE_SYS.h"

// --- other-subsystem / global boundaries (declared, not reversed here) ---
// DEVIATION (2026-07-31): apConsole is really con::MSG_PROTOCOL* (per DB / aiCON_CB_globals.h),
// not an opaque con_CONSOLE_SYS. The concrete object is a con::CONSOLE_SYS (subclass); the
// LINE_START Update(-1) call reaches con::CONSOLE_SYS::Update via a cast (see below).
extern con::MSG_PROTOCOL *apConsole;             // ap: active debug console (may be null)
extern sslSYSTEM       *gsSslSystem;             // gs: global ssl system (owns errorCallStack)
extern const char      *dsUnaryOpName[];         // ds: printable unary-op names
extern const bool       IGNORE_STRONG_ASSERT;    // build-time assert gate
extern dsTSTRING<char>  dsSPrintf(const char *fmt, ...);           // ds: formatted string build (boundary)
extern void             osSleep(int ms);                           // os: sleep helper (boundary)

// NULL runtime-type storage used to build null dsDATA values (ds boundary).
extern const dsDATA_TYPE *dsDATA_NULL_TYPE();    // == dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType
extern const dsDATA_TYPE *dsDATA_STRING_TYPE();  // == dsDATA_TYPE_STORAGE<dsTSTRING<char>>::dataType
extern const dsDATA_TYPE *dsDATA_OBJREF_TYPE();  // == dsDATA_TYPE_STORAGE<sslOBJ_REF>::dataType

sslERROR sslSCRIPTED_FUNCTION_BASE::ExecCode(sslOBJECT *pSelf, sslSTACK &stack,
                                             int /*numParams*/, int paramBase,
                                             dsDATA *retVal, sslOBJECT * /*pCaller*/) // 6th arg is sslOBJECT* per DB mangling
{
    // Owning ssl system: resolve from the defining class (or self's class as a fallback).
    sslCLASS *definingClass = this->pClassOfDefinition ? this->pClassOfDefinition
                                                       : pSelf->refClass.pClass;
    sslSYSTEM *ssl = definingClass->pSslSystem; // sslCLASS.pSslSystem @0x04 (DB)
    sslOBJECT *globalObj = ssl->GetGlobalObj().pObject;                // shared "global" object instance

    const char *localsBase = (const char *)stack.GetLocalVarsOfs();    // base of this frame's locals
    // DEVIATION (2026-07-31): funcName is used by value (`.CStr()`) at all 16 sites below, so it
    // is a reference to the decl's leading dsSTRID name field, not a pointer.
    const dsSTRID &funcName = *(const dsSTRID *)this->pDecl;           // decl's leading field is the name

    sslERROR err;        // per-instruction result
    sslERROR firstErr;   // first recoverable error, returned on normal completion
    bool  finished = false;   // set by RETURN_* to stop the loop
    int   ip = 0;             // instruction pointer
    float lastDelay = 0.0f;   // last delayed-call value, feeds SSL_BC_PUSH_LAST_DELAY

    if (this->bytecode.nElem <= 0)
        return firstErr;

    while (!finished)
    {
        const sslBYTECODE_INSTRUCTION &instr = this->bytecode[ip];

        switch (instr.cmd)
        {
        case SSL_BC_PUSH_NULL:
        {
            dsDATA nullVal(dsDATA_NULL_TYPE());
            stack.Push().SetToDsData(nullVal);
            break;
        }

        case SSL_BC_PUSH_INT:
            stack.Push().SetToInt(instr.i_param);
            break;

        case SSL_BC_PUSH_BOOL:
            stack.Push().SetToBool(instr.i_param != 0);
            break;

        case SSL_BC_PUSH_FLOAT:
            stack.Push().SetToFloat(instr.f_param);
            break;

        case SSL_BC_PUSH_DSSTRID:
            stack.Push().SetToDsStrId(*(dsSTRID *)&instr.i_param);
            break;

        case SSL_BC_PUSH_FUNC_CONST_REF:
            // reference into this function's const pool (consts is result[1])
            stack.Push().SetToFuncConstRef((dsDATA *)&this->consts, instr.i_param);
            break;

        case SSL_BC_PUSH_FUNC_PARAM_REF:
            stack.Push().SetToFuncParamRefStack(&stack, instr.i_param + paramBase);
            break;

        case SSL_BC_PUSH_LOCAL_VAR_REF:
            stack.Push().SetToLocalVarRef(&stack, (int)&localsBase[instr.i_param]);
            break;

        case SSL_BC_PUSH_STACK_VAL:
        {
            // duplicate the operand at depth i_param below the top
            sslSTACK_DATA dup(stack.At(stack.mainStack.nElem - instr.i_param - 1));
            stack.Push() = dup;
            break;
        }

        case SSL_BC_PUSH_PARENT_REF:
        {
            sslCLASS *defClass = this->pClassOfDefinition; // result->desc.pBuffer
            if (!defClass)
                return sslERROR(SSL_ERR_RUNTIME,
                                dsTSTRING<char>("can't use 'parent' in standalone funcs"), 0);
            sslCLASS *parentClass = defClass->refParent.pClass; // sslCLASS.refParent @0x08 (DB)
            if (!parentClass)
                return sslERROR(SSL_ERR_RUNTIME,
                                dsTSTRING<char>("can't get parent of the top-level class"), 0);
            stack.Push().SetToObjRef(pSelf, parentClass);
            break;
        }

        case SSL_BC_PUSH_SELF_REF:
        {
            sslCLASS *cls;
            if (instr.i_param)
            {
                cls = this->pClassOfDefinition;              // result->desc.pBuffer
                if (!cls) cls = pSelf->refClass.pClass;
            }
            else
            {
                cls = pSelf->refClass.pClass;
            }
            stack.Push().SetToObjRef(pSelf, cls);
            break;
        }

        case SSL_BC_PUSH_LAST_DELAY:
            stack.Push().SetToFloat(lastDelay);
            break;

        case SSL_BC_POP_VALUE:
            stack.Pop();
            break;

        case SSL_BC_CONST_NAME_TO_OBJREF:
        {
            // resolve a const string identifier to a named object instance
            dsDATA &nameConst = this->consts[instr.i_param];
            dsTSTRING<char> objName(*(dsTSTRING<char> *)nameConst.RawWrapperPtr()); // const string value
            sslSYSTEM *sslForClass = pSelf->refClass.GetSslSystem();
            sslOBJECT *found = sslForClass->FindObject(objName).pObject;
            if (found)
            {
                stack.Push().SetToObjRef(found, found->refClass.pClass);
            }
            else
            {
                // recoverable: record first error, push a null object ref, keep going
                sslERROR notFound(SSL_ERR_RUNTIME,
                    dsSPrintf("$%s.%s(): trying to get subref '%s' of the NULL object",
                              pSelf->GetDbgName().CStr(), funcName.CStr(), objName.CStr()),
                    0);
                err = notFound;
                if (firstErr.id == SSL_ERR_NO_ERROR) firstErr = err;
                stack.Push().SetToObjRef(nullptr, nullptr);
            }
            break;
        }

        case SSL_BC_IDENT_TO_SELF_GLOBAL:
        {
            // self-patching identifier resolution. Two following HINT instructions cache the
            // resolved (class ptr, sub-element idx). i_param carries the identifier name id.
            dsSTRID identName = *(dsSTRID *)&instr.i_param;
            sslBYTECODE_INSTRUCTION &hint1 = this->bytecode[ip + 1];
            sslBYTECODE_INSTRUCTION &hint2 = this->bytecode[ip + 2];
            ip += 2; // consume the two hint slots

            sslCLASS *cachedClass = (sslCLASS *)hint1.i_param;
            if (cachedClass == pSelf->refClass.pClass)
            {
                int cachedIdx = hint2.i_param;
                if (cachedIdx == -1)
                    return sslERROR(SSL_ERR_RUNTIME,
                        dsSPrintf("$%s.%s(): identifier '%s' is not found",
                                  pSelf->GetDbgName().CStr(), funcName.CStr(),
                                  identName.CStr()),
                        0);
                if (cachedIdx >= 0)
                {
                    if ((cachedIdx & 0x40000000) != 0 && this->rContainerObject.pObject)
                    {
                        // resolve against the container object
                        sslCLASS *ownerClass = this->rContainerObject.GetClass().pClass;
                        sslOBJECT *ownerObj = this->rContainerObject.pObject;
                        stack.Push().SetToObjSubElemRef(ownerObj, ownerClass,
                                                        hint2.i_param & 0x0FFFFFFF);
                    }
                    else
                    {
                        stack.Push().SetToObjSubElemRef(pSelf, cachedClass, cachedIdx);
                    }
                }
                else
                {
                    // negative (with 0x8000_0000 tag) -> resolve against the global object
                    stack.Push().SetToObjSubElemRef(globalObj, globalObj->refClass.pClass,
                                                    cachedIdx & 0x0FFFFFFF);
                }
                break;
            }

            // cold path: (re)resolve the name, patch hint1 (class) and hint2 (idx).
            hint1.i_param = (int)pSelf->refClass.pClass;
            int selfIdx = pSelf->refClass.pClass->GetSubNameIdx(identName);
            if (selfIdx != -1)
            {
                stack.Push().SetToObjSubElemRef(pSelf, pSelf->refClass.pClass, selfIdx);
                hint2.i_param = selfIdx;
                break;
            }
            if (this->rContainerObject.pObject)
            {
                sslCLASS *containerClass = this->rContainerObject.GetClass().pClass;
                int containerIdx = containerClass->GetSubNameIdx(identName);
                if (containerIdx != -1)
                {
                    stack.Push().SetToObjSubElemRef(this->rContainerObject.pObject,
                                                    containerClass, containerIdx);
                    hint2.i_param = containerIdx | 0x40000000;
                    break;
                }
            }
            int globalIdx = globalObj->refClass.pClass->GetSubNameIdx(identName);
            if (globalIdx != -1)
            {
                stack.Push().SetToObjSubElemRef(globalObj, globalObj->refClass.pClass, globalIdx);
                hint2.i_param = globalIdx | 0x80000000;
                break;
            }
            return sslERROR(SSL_ERR_RUNTIME,
                dsSPrintf("$%s.%s(): identifier '%s' is not found",
                          pSelf->GetDbgName().CStr(), funcName.CStr(),
                          identName.CStr()),
                0);
        }

        case SSL_BC_IDENT_TO_SELF_GLOBAL_HINT1:
        case SSL_BC_IDENT_TO_SELF_GLOBAL_HINT2:
            // hint slots are consumed by SSL_BC_IDENT_TO_SELF_GLOBAL; reaching one standalone is a bug
            break;

        case SSL_BC_GET_SUB_REF:
        case SSL_BC_GET_SUB_REF_FROM_STACK_NAME:
        {
            dsSTRID memberName;
            if (instr.cmd == SSL_BC_GET_SUB_REF)
            {
                memberName = *(dsSTRID *)&instr.i_param;
            }
            else
            {
                // member name comes off the stack as a string expression
                dsDATA nameData;
                err = stack.GetTop().GetAsDsData(nameData);
                if (err.id) return err;
                if (!nameData.Convert(dsDATA_STRING_TYPE(), DSD_CONV_MATH))
                    return sslERROR(SSL_ERR_GENERIC,
                                    dsTSTRING<char>("subref expression not a string"), 0);
                memberName = dsSTRID(*(const dsTSTRING<char> *)nameData.RawWrapperPtr(), true);
                stack.Pop();
            }

            dsDATA target;
            err = stack.GetTop().GetAsDsData(target);
            const dsDATA_TYPE *tt = target.type;
            if (tt && tt != dsDATA_OBJREF_TYPE()
                   && (tt->IsFunc() || tt->IsProperty(&target, memberName)))
            {
                // a dsDATA member/property/function reference
                stack.Pop();
                stack.Push().SetToDsDataMemberRef(target, memberName);
            }
            else
            {
                // resolve as an object sub-element reference
                sslOBJECT *obj = nullptr;
                sslCLASS  *objClass = nullptr;
                err = stack.GetTop().GetAsObjRef(obj, objClass, (sslSYSTEM *)ssl);
                stack.Pop();
                if (err.id) return err;

                if (obj && objClass)
                {
                    int subIdx = objClass->GetSubNameIdx(memberName);
                    if (((obj->state.state >> 4) & 1) == 0 && subIdx == -1) /* typed: sslOBJECT::state @0x08 (all 4 sites) */
                        return sslERROR(SSL_ERR_RUNTIME,
                            dsSPrintf("$%s.%s(): identifier '%s' is not found",
                                      pSelf->GetDbgName().CStr(), funcName.CStr(),
                                      memberName.CStr()),
                            0);
                    stack.Push().SetToObjSubElemRef(obj, objClass, subIdx);
                }
                else
                {
                    // recoverable: record first error, push a null sub-element ref
                    err = sslERROR(SSL_ERR_RUNTIME,
                        dsSPrintf("$%s.%s(): trying to get subref '%s' of the NULL object",
                                  pSelf->GetDbgName().CStr(), funcName.CStr(),
                                  memberName.CStr()),
                        0);
                    if (firstErr.id == SSL_ERR_NO_ERROR) firstErr = err;
                    stack.Push().SetToObjSubElemRef(nullptr, nullptr, -1);
                }
            }
            break;
        }

        case SSL_BC_CALL_FUNCTION:
        {
            int argc      = (unsigned short)instr.i_param;
            int hasNamed  = ((unsigned int)instr.i_param >> 30) & 1;

            if (stack.At(stack.mainStack.nElem - argc - 1).type == sslSTACK_DATA::TYPE_DSDATA_MEMBER_REF)
            {
                // call a member function on a dsDATA value
                dsDATA  target;
                dsSTRID methodName;
                err = stack.At(stack.mainStack.nElem - argc - 1).GetAsDsDataMemberRef(target, methodName);
                if (err.id) return err;

                dsDATA retData;
                dsVECTOR<dsDATA, 8> args;
                args.Resize(argc);
                for (int i = 0, slot = argc - 1; i < argc; ++i, --slot)
                {
                    err = stack.GetTop().GetAsDsData(args[slot]);
                    if (err.id) return err;
                    stack.Pop();
                }

                int   nArgs   = args.nElem;
                dsDATA *argPtr = args.nElem ? &args[0] : nullptr;
                if (!target.type->CallFunc(methodName, &target, argPtr, nArgs, &retData))
                {
                    dsTSTRING<char> targetDbg = target.GetDbgString();
                    dsTSTRING<char> selfDbg   = pSelf->GetDbgName();
                    const char *typeName = target.type ? target.type->GetDbgName() : "null";
                    dsTSTRING<char> msg = dsSPrintf(
                        "$%s.%s(): Failed to call function '%s' for dsDATA object '%s':'%s'",
                        selfDbg.CStr(), funcName.CStr(), methodName.CStr(),
                        typeName, targetDbg.CStr());
                    if (dsERROR_HANDLER<dsDATA>::IsError(&target))
                    {
                        dsTSTRING<char> detail = dsERROR_HANDLER<dsDATA>::GetError(&target);
                        msg += (dsTSTRING<char>(", err: ") + detail);
                    }
                    return sslERROR(SSL_ERR_RUNTIME, msg, 0);
                }
                stack.Pop();
                stack.Push().SetToDsData(retData);
            }
            else
            {
                // call an object sub-element (script/native) function
                sslOBJECT *obj = nullptr;
                sslCLASS  *objClass = nullptr;
                int        funcIdx = 0;
                err = stack.At(stack.mainStack.nElem - argc - 1)
                          .GetAsObjSubElemRef(obj, objClass, funcIdx, (sslSYSTEM *)ssl);
                if (err.id) return err;

                if (funcIdx >= 0)
                {
                    dsDATA retData(dsDATA_NULL_TYPE());
                    if (obj && objClass && ((obj->state.state >> 4) & 1) == 0)
                    {
                        if (hasNamed)
                        {
                            err = sslFUNCTION_BASE::TransferNamedParams(objClass, funcIdx, argc, stack);
                            if (err.id) return err;
                            argc = objClass->GetFunctionDecl(funcIdx)->param.nElem;
                        }
                        err = objClass->CallFunc(obj, funcIdx, argc, nullptr, &retData, pSelf);
                        if (err.id) return err;
                    }
                    else if (argc > 0)
                    {
                        do { stack.Pop(); } while (--argc);
                    }
                    stack.Pop(); // pop the function reference

                    if (obj && objClass && ((obj->state.state >> 4) & 1) != 0)
                    {
                        // native/host object: coerce the return to the declared return type
                        if (objClass->GetFunctionDecl(funcIdx)->pRetType)
                        {
                            dsDATA typed(objClass->GetFunctionDecl(funcIdx)->pRetType);
                            retData.StoreValue(&typed);
                        }
                    }
                    stack.Push().SetToDsData(retData);
                }
                else
                {
                    // suppressed reference: pop it and push null
                    stack.Pop();
                    dsDATA nullVal(dsDATA_NULL_TYPE());
                    stack.Push().SetToDsData(nullVal);
                }
            }
            break;
        }

        case SSL_BC_CALL_FUNCTION_DELAYED:
        {
            int argc     = (unsigned short)instr.i_param;
            int hasNamed = ((unsigned int)instr.i_param >> 30) & 1;

            sslOBJECT *obj = nullptr;
            sslCLASS  *objClass = nullptr;
            int        funcIdx = 0;
            err = stack.At(stack.mainStack.nElem - argc - 3)
                      .GetAsObjSubElemRef(obj, objClass, funcIdx, (sslSYSTEM *)ssl);
            if (err.id) return err;

            if (funcIdx >= 0)
            {
                if (obj && objClass && ((obj->state.state >> 4) & 1) == 0)
                {
                    // group name: taken as a const bool marker or a string expression
                    dsSTRID groupName;
                    if (stack.GetTop().type == sslSTACK_DATA::TYPE_BOOL)
                    {
                        groupName = dsSTRID();
                    }
                    else
                    {
                        dsDATA nameData;
                        err = stack.GetTop().GetAsDsData(nameData);
                        if (err.id) return err;
                        if (!nameData.Convert(dsDATA_STRING_TYPE(), DSD_CONV_EXPLICIT))
                            return sslERROR(SSL_ERR_RUNTIME,
                                dsSPrintf("must use CONST name or string expression as delayed "
                                          "function group in '$%s.%s'",
                                          pSelf->GetDbgName().CStr(), funcName.CStr()),
                                0);
                        groupName = dsSTRID(*(const dsTSTRING<char> *)nameData.RawWrapperPtr(), false);
                    }
                    stack.Pop();

                    float delay = 0.0f;
                    err = stack.GetTop().GetAsFloat(delay);
                    lastDelay = delay;
                    if (err.id) return err;
                    stack.Pop();

                    if (hasNamed)
                    {
                        err = sslFUNCTION_BASE::TransferNamedParams(objClass, funcIdx, argc, stack);
                        argc = objClass->GetFunctionDecl(funcIdx)->param.nElem;
                    }

                    sslOBJ_REF targetRef;
                    sslOBJ_REF selfRef;
                    targetRef._SetObjectPtr(obj);
                    selfRef._SetObjectPtr(pSelf);
                    // DEVIATION (2026-07-31): reordered to the DB signature
                    // AddDelayedFunc(obj, funcIdx, caller, timeOfs, argc, stack, group). Decompiler
                    // (@0x82ADAB70 line 1554/1555) shows obj=target object, caller=pSelf.
                    ssl->AddDelayedFunc(targetRef, funcIdx, selfRef, delay, argc, stack, groupName);
                }
                else
                {
                    // host/native object cannot be delayed: drop args + name + delay
                    if (argc > 0)
                        do { stack.Pop(); } while (--argc);
                    stack.Pop();
                    stack.Pop();
                }
            }
            else
            {
                // suppressed reference: pop it and push null
                stack.Pop();
                dsDATA nullVal(dsDATA_NULL_TYPE());
                stack.Push().SetToDsData(nullVal);
            }
            break;
        }

        case SSL_BC_RETURN_VALUE:
            if (retVal)
            {
                err = stack.GetTop().GetAsDsData(*retVal);
                if (err.id != SSL_ERR_NO_ERROR)
                    return err;
            }
            stack.Pop();
            finished = true;
            break;

        case SSL_BC_RETURN_VOID:
            if (retVal)
            {
                dsDATA nullVal(dsDATA_NULL_TYPE());
                nullVal.StoreValue(retVal);
            }
            finished = true;
            break;

        case SSL_BC_POP_JUMP_IF_FALSE:
        {
            bool cond = false;
            err = stack.GetTop().GetAsBool(cond);
            stack.Pop();
            if (err.id) return err;
            if (!cond) ip = instr.i_param - 1;
            break;
        }

        case SSL_BC_POP_JUMP_IF_TRUE:
        {
            bool cond = false;
            err = stack.GetTop().GetAsBool(cond);
            stack.Pop();
            if (err.id) return err;
            if (cond) ip = instr.i_param - 1;
            break;
        }

        case SSL_BC_PEEK_JUMP_IF_FALSE:
        {
            bool cond = false;
            err = stack.GetTop().GetAsBool(cond);
            if (err.id) return err;
            if (!cond) ip = instr.i_param - 1;
            break;
        }

        case SSL_BC_PEEK_JUMP_IF_TRUE:
        {
            bool cond = false;
            err = stack.GetTop().GetAsBool(cond);
            if (err.id) return err;
            if (cond) ip = instr.i_param - 1;
            break;
        }

        case SSL_BC_JUMP:
            ip = instr.i_param - 1;
            break;

        case SSL_BC_UN_MATH_OP:
        {
            dsDATA operand;
            err = stack.GetTop().GetAsDsData(operand);
            if (err.id) return err;
            if (operand.Math((dsDATA_UNARY_OP)instr.i_param) != DSD_MATH_OK)
            {
                const char *typeName = operand.type ? operand.type->GetDbgName() : "null";
                return sslERROR(SSL_ERR_RUNTIME,
                    dsSPrintf("can't perform '%s' operation on value of type <%s> in '$%s.%s'",
                              dsUnaryOpName[instr.i_param], typeName,
                              pSelf->GetDbgName().CStr(), funcName.CStr()),
                    0);
            }
            stack.GetTop().SetToDsData(operand);
            break;
        }

        case SSL_BC_BIN_MATH_OP:
            err = this->MathFromStackToStack(pSelf, stack, (dsDATA_BINARY_OP)instr.i_param);
            if (err.id != SSL_ERR_NO_ERROR)
                return err;
            break;

        case SSL_BC_CONVERT_TYPE:
        {
            dsDATA value;
            err = stack.GetTop().GetAsDsData(value);
            if (err.id) return err;
            const dsDATA_TYPE *targetType = this->usedTypes[instr.i_param];
            if (!value.Convert(targetType, DSD_CONV_EXPLICIT))
            {
                const dsDATA_TYPE *dstType = this->usedTypes[instr.i_param];
                const char *fromName = value.type ? value.type->GetDbgName() : "null";
                const char *toName   = dstType->GetDbgName();
                return sslERROR(SSL_ERR_RUNTIME,
                    dsSPrintf("can't convert from type '%s' to '%s' in '$%s.%s'",
                              fromName, toName, pSelf->GetDbgName().CStr(), funcName.CStr()),
                    0);
            }
            stack.GetTop().SetToDsData(value);
            break;
        }

        case SSL_BC_FOREACH_INIT:
        {
            dsSTRID className = *(dsSTRID *)&instr.i_param;
            sslCLASS_REF classRef = ssl->FindClass(className);
            stack.AllocForeach(classRef);
            if (classRef.pClass)
            {
                stack.Push().SetToBool(true);
            }
            else
            {
                // recoverable: record first error, push false (loop body will be skipped)
                err = sslERROR(SSL_ERR_RUNTIME,
                    dsSPrintf("unknown class in foreach <%s> in '$%s.%s'",
                              className.CStr(), pSelf->GetDbgName().CStr(),
                              funcName.CStr()),
                    0);
                if (firstErr.id == SSL_ERR_NO_ERROR) firstErr = err;
                stack.Push().SetToBool(false);
            }
            break;
        }

        case SSL_BC_FOREACH_TERM:
            stack.FreeTopForeach();
            break;

        case SSL_BC_FOREACH_ITERATE:
        {
            sslFOREACH_DATA &cursor = stack.GetTopForEach();
            sslCLASS_REF filter(cursor.checkClass);
            // DEVIATION (2026-07-31): FindNextObject returns sslOBJ_REF by value; DB sig is
            // FindNextObject(int& curIdx, sslCLASS_REF cls) (decompiler @0x82ADAB70 line 1982).
            sslOBJ_REF nextRef = ssl->FindNextObject(cursor.lastObjIdx, filter);
            dsDATA nextVal = (dsDATA)nextRef;
            stack.SetLocalVar((int)&localsBase[instr.i_param], nextVal);
            bool haveNext = (ssl->lastErr.id == SSL_ERR_NO_ERROR);
            stack.Push().SetToBool(haveNext);
            break;
        }

        case SSL_BC_RESET_LOCAL_VAR:
        {
            int typeId = 0;
            stack.GetTop().GetAsInt(typeId);
            stack.Pop();
            if (typeId < 0)
                stack.ResetLocalVar(nullptr, (int)&localsBase[instr.i_param]);
            else
                stack.ResetLocalVar(this->usedTypes[typeId], (int)&localsBase[instr.i_param]);
            break;
        }

        case SSL_BC_ASSIGN:
        {
            dsDATA value;
            err = stack.GetTop().GetAsDsData(value);
            stack.Pop();
            if (err.id) return err;
            err = stack.GetTop().SetRefTo(value);
            if (err.id) return err;
            stack.Pop();
            break;
        }

        case SSL_BC_NAMED_PARAM:
        {
            dsSTRID paramName = *(dsSTRID *)&instr.i_param;
            stack.namedParams.PushBack(paramName);
            break;
        }

        case SSL_BC_DELAYED_ACTION:
        {
            // group name: const bool marker or a string expression
            dsSTRID groupName;
            if (stack.GetTop().type == sslSTACK_DATA::TYPE_BOOL)
            {
                groupName = dsSTRID();
            }
            else
            {
                dsDATA nameData;
                err = stack.GetTop().GetAsDsData(nameData);
                if (err.id) return err;
                if (!nameData.Convert(dsDATA_STRING_TYPE(), DSD_CONV_EXPLICIT))
                    return sslERROR(SSL_ERR_RUNTIME,
                        dsSPrintf("must use CONST name or string expression as delayed function "
                                  "group in '$%s.%s'",
                                  pSelf->GetDbgName().CStr(), funcName.CStr()),
                        0);
                groupName = dsSTRID(*(const dsTSTRING<char> *)nameData.RawWrapperPtr(), false);
            }
            stack.Pop();

            // target object: bool marker (self/none) or an object ref
            sslOBJECT *actionObj = nullptr;
            if (stack.GetTop().type == sslSTACK_DATA::TYPE_BOOL)
            {
                actionObj = nullptr;
            }
            else
            {
                sslCLASS *actionClass = nullptr;
                err = stack.GetTop().GetAsObjRef(actionObj, actionClass, (sslSYSTEM *)ssl);
                if (err.id) return err;
            }
            stack.Pop();

            int action = 0;
            err = stack.GetTop().GetAsInt(action);
            if (err.id != SSL_ERR_NO_ERROR)
                return err;
            stack.Pop();
            sslOBJ_REF actionRef(actionObj);
            ssl->DelayedFuncAction((sslSYSTEM::DELACT)action, groupName, actionRef);
            break;
        }

        case SSL_BC_SUBREF_TO_CONST_NAME:
        {
            if (stack.GetTop().type != sslSTACK_DATA::TYPE_SSLOBJECT_SUBELEM_REF)
                return sslERROR(SSL_ERR_RUNTIME,
                    dsSPrintf("expected use CONST name or string expression in '$%s.%s'",
                              pSelf->GetDbgName().CStr(), funcName.CStr()),
                    0);

            sslOBJECT *obj = nullptr;
            sslCLASS  *objClass = nullptr;
            int        subIdx = -1;
            err = stack.GetTop().GetAsObjSubElemRef(obj, objClass, subIdx, (sslSYSTEM *)ssl);
            if (err.id) return err;

            if (subIdx < 0)
                return sslERROR(SSL_ERR_RUNTIME,
                    dsSPrintf("can't convert suppressed reference to CONST name in '$%s.%s'",
                              pSelf->GetDbgName().CStr(), funcName.CStr()),
                    0);
            if (!obj || !objClass)
                return sslERROR(SSL_ERR_RUNTIME,
                    dsSPrintf("can't convert null ref to CONST name in '$%s.%s'",
                              pSelf->GetDbgName().CStr(), funcName.CStr()),
                    0);
            if (!objClass->IsSubElemConst(subIdx))
                return sslERROR(SSL_ERR_RUNTIME,
                    dsSPrintf("expected use CONST name or string expression in '$%s.%s'",
                              pSelf->GetDbgName().CStr(), funcName.CStr()),
                    0);

            dsSTRID elemName = objClass->GetSubElemName(subIdx);
            dsTSTRING<char> nameStr(elemName.CStr());
            dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > nameWrap(nameStr);
            dsDATA nameData = (dsDATA)nameWrap;
            stack.GetTop().SetToDsData(nameData);
            break;
        }

        case SSL_BC_LINE_START:
        {
            // source-line marker / interactive-debugger breakpoint check. Best-effort: several
            // sub-fields of the source object are read through offsets the decompiler typed loosely.
            if (!apConsole || !apConsole->isConnected)
                break;
            sslCLASS *srcClass = this->pClassOfDefinition; // result->desc.pBuffer
            if (!srcClass)
                break;
            int line = instr.i_param;
            sslSOURCE_REF srcRef = srcClass->GetSource();
            sslSYSTEM *dbg = srcClass->pSslSystem; // typed (sslCLASS::pSslSystem @0x04) owning system w/ debug state
            if (dbg->dbgMode == 4)
                dbg->dbgMode = 2;
            int mode = dbg->dbgMode;
            // DB: sslCLASS+0x14 = nameToElemIdx.nElem (dsVECTOR nElem @+4); the decompiler passes
            // this count as IsBreakpoint's second id argument.
            int sourceId = srcClass->nameToElemIdx.nElem;
            // DEVIATION (2026-07-31): IsBreakpoint is a member of sslSOURCE, reached through
            // srcRef.source (the sslSOURCE*); srcRef.Get() returns the source TEXT (dsTSTRING),
            // not the source object (decompiler @0x82ADAB70 line 2416).
            if (mode != 2 && mode != 5
                && (!srcRef.source || !srcRef.source->IsBreakpoint(line, sourceId, pSelf->uid)))
            {
                break; // no breakpoint here
            }
            // hand off to the debugger: publish state, notify, and spin until it releases us.
            // DEVIATION (2026-07-31): BuildSource returns int (adjusted line, or -1) and takes 8
            // args (sslSOURCE_REF*, TWO dsVECTOR<int,8>*, dsTSTRING<char>* out-text). The mode-2
            // call fills the assembled source text used as the StartMsg name string (decompiler
            // @0x82ADAB70 lines 2424-2440). Best-effort: this debugger path's field typing is loose.
            dsTSTRING<char> builtSourceName;
            int built = dbg->BuildSource(sourceId, pSelf->uid, line, 2,
                                         nullptr, nullptr, nullptr, &builtSourceName);
            if (built == -1)
                break;
            for (;;)
            {
                // DEVIATION (2026-07-31): StartMsg returns fioFILE& and SendImmMsg takes fioFILE&
                // (DB con::MSG_PROTOCOL sigs); use a reference and member-call syntax.
                fioFILE &msg = apConsole->StartMsg("Ssl", "SSL_Dlg", builtSourceName.CStr());
                msg.WriteData(&built, 4, 4);
                int srcIdField = sourceId;
                msg.WriteData(&srcIdField, 4, 4);
                int uidField = pSelf->uid;
                msg.WriteData(&uidField, 4, 4);
                if (apConsole->SendImmMsg(msg))
                {
                    dbg->dbgMode         = 1;
                    dbg->dbgBCPosition   = ip;
                    dbg->dbgFunc         = this;
                    dbg->dbgParamStackPos = paramBase;
                    dbg->dbgObject       = pSelf->uid;
                    if (!apConsole->isConnected)
                        break;
                    for (;;)
                    {
                        osSleep(10);
                        // DEVIATION (2026-07-31): the concrete apConsole object is a con::CONSOLE_SYS;
                        // Update(int) lives on that subclass (decompiler: con::CONSOLE_SYS::Update).
                        ((con::CONSOLE_SYS *)apConsole)->Update(-1);
                        mode = dbg->dbgMode;
                        if (mode != 1)
                            break;
                        if (!apConsole->isConnected)
                            goto line_start_done;
                    }
                }
                if (mode != 5 || !apConsole->isConnected)
                    break;
            }
        line_start_done:
            break;
        }

        default:
            break;
        }

        // advance to the next instruction (jumps set ip = target-1 above)
        if (++ip >= this->bytecode.nElem)
            break;
    }

    // normal completion: on a recorded soft error, drop the error call stack; return the first error.
    if (firstErr.id)
        gsSslSystem->errorCallStack.Clear();
    return firstErr;
}
