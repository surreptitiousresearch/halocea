#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA.h"
#include "sslFUNCTION_BASE.h"
#include "sslOBJ_REF.h"
#include "sslBYTECODE_INSTRUCTION.h"
#include "../ds/dsDATA_MATH.h"
// ssl subsystem: base of a compiled/parsed script function. DB-verified layout
// (types_members sslSCRIPTED_FUNCTION_BASE): anonymous sslFUNCTION_BASE@0, consts@12,
// usedTypes@32, bytecode@52, rContainerObject@72, localVars@76 — size 96.
// The anonymous leading sslFUNCTION_BASE member is modeled as public inheritance (matches layout,
// promotes pDecl/pClassOfDefinition/stateFunc). The dsVECTOR members store T* internally, so their
// still-opaque element types only need forward declarations.

struct dsDATA_TYPE;               // ds runtime type descriptor (full definition in ds/dsDATA_TYPE.h)
struct sslOBJECT;                 // ssl script object instance (full definition in sslOBJECT.h)
struct sslSTACK;                  // ssl evaluation stack (full definition in sslSTACK.h)
struct sslERROR;                  // ssl error/result value (full definition in sslERROR.h)

struct sslSCRIPTED_FUNCTION_BASE : sslFUNCTION_BASE {
    struct LOCAL_VAR {
        dsTSTRING<char> name;       // 0x00 declared variable name
        int             typeId;     // 0x04 declared type id
        int             declarePos; // 0x08 bytecode position of the declaration
        int             stackPos;   // 0x0C slot index on the locals stack
    };

    dsVECTOR<dsDATA, 8>                    consts;           // 0x0C constant pool
    dsVECTOR<const dsDATA_TYPE *, 8>       usedTypes;        // 0x20 referenced runtime types
    dsVECTOR<sslBYTECODE_INSTRUCTION, 8>   bytecode;         // 0x34 compiled instructions
    sslOBJ_REF                             rContainerObject; // 0x48 owning object ref
    dsVECTOR<LOCAL_VAR, 8>                 localVars;        // 0x4C declared locals

protected:
    // 0x82AD8470 — pop the top two operands off the operand `stack`, apply binary operator `op`
    // (left op right), and push the result. Returns sslERROR by value (sret). Protected (IAA).
    sslERROR MathFromStackToStack(sslOBJECT *pSelf, sslSTACK &stack, dsDATA_BINARY_OP op);

    // 0x82ADAB70 — execute this function's compiled bytecode on `pSelf`, with `numParams` arguments
    // based at stack index `paramBase`, writing the result into `*retVal`. `pCaller` (the 6th arg —
    // DB mangling back-ref 0 = sslOBJECT*, not dsDATA*) is unreferenced by the body. Returns sslERROR
    // by value (sret) — the first soft error accumulated, or a no-error result. Protected (IAA).
    sslERROR ExecCode(sslOBJECT *pSelf, sslSTACK &stack, int numParams, int paramBase,
                      dsDATA *retVal, sslOBJECT *pCaller);

    // 0x825BBAD8 — explicit member-wise teardown in reverse declaration order (localVars, then
    // rContainerObject, bytecode, usedTypes, consts), then chain to the sslFUNCTION_BASE base dtor.
    ~sslSCRIPTED_FUNCTION_BASE();
};
