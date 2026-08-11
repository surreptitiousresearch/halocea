#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ent/entCREATE_INFO.h"
#include "../../headers/ws/ent/entDESC.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/scn/scnSCENE.h"
#include "../../headers/ws/gs/gsLVL_SYSTEM.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/dsc/dscDESC_FAMILY.h"
#include "../../headers/ws/msg/msgSYSTEM.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/entCREATE_DATA.h"
#include <string.h>

extern "C" void dlFree(void *ptr);
void _apLog(const char *format, ...); // boundary — ap log sink

// ws-engine gs boundary globals (same convention as entENTITY::Register).
extern gsLVL_SYSTEM *gsSysLevel;
extern scnSCENE      *gsScenePtr; // ?gsScenePtr@@3PAVscnSCENE@@A

// Save/restore stack of the entity id currently under construction (re-entrancy guard for
// nested entCreate calls triggered from within a create-data/descriptor callback).
extern "C" int gs_creatingEntityId;

// Process-wide per-class-family instrumentation counters (apCOUNTER_TIME_CALL), stamped onto
// newly-created entities of well-known base classes.
extern "C" apCOUNTER_TIME_CALL cnt_plr;
extern "C" apCOUNTER_TIME_CALL cnt_pjl;
extern "C" apCOUNTER_TIME_CALL cnt_wpn;

// ia (interactive actor) subsystem: process-wide actor-family factory registry. BOUNDARY — not
// reversed in this batch; only the one instantiation entCreate needs is modeled.
struct iaFAMILY {
    // ??$CreateIA@VentENTITY@@@iaFAMILY@@QAAPAVentENTITY@@ABV?$dsTSTRING@D@@@Z @ 0x82535FF0 —
    // allocate and construct a fresh entENTITY registered under the named class brand.
    entENTITY *CreateIA(const dsTSTRING<char> &nameClass); // boundary
};
extern iaFAMILY *iaFamily; // ?iaFamily@@3PAViaFAMILY@@A

// ??$Desc@VentENTITY@@@@YAPAVentDESC@@PBVentENTITY@@@Z @ 0x823D3BF0 — free template helper that
// returns the live object's descriptor (spDesc.pointee, downcast to entDESC). Same-family
// utility, not reversed in this batch. boundary.
extern entDESC *Desc(const entENTITY *self);

// operator+<char>(const dsTSTRING<char>&, const char*) — concatenate a string literal onto a
// dsTSTRING<char>, returned by value (sret). Free template helper, not reversed in this batch.
// boundary.
extern dsTSTRING<char> operator+(const dsTSTRING<char> &a, const char *b);

// entCreate(animINST*, const entCREATE_INFO&) — 0x82536290
//
// CAVEAT: dsTSTRING<char>::operator= and dsTSTRING<char>::ReleaseBuffer are compiler-INLINED
// throughout this function (protected/no-`bl` at every adoption or teardown point) --
// reconstructed as the equivalent manual release-old/adopt-new/bump-refcount sequences,
// matching the convention established for this pattern elsewhere in this batch (see
// entCREATE_INFO::entCREATE_INFO and propINST_CONSTRUCTOR::ChangeSkin).
entENTITY *entCreate(animINST *pInst, const entCREATE_INFO &info)
{
    if (gsSysLevel && !IGNORE_STRONG_ASSERT && (gsSysLevel->stateLvl.state & 4) != 0)
        // Crash is a non-static member (this passed null by the original); call on a null instance
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "!gsSysLevel->IsProcessDestroyLevel()",
            "D:\\Projects\\code\\common\\src.sys\\gm_sys\\entity\\entity.cpp",
            585,
            empty_string);

    int savedCreatingEntityId = gs_creatingEntityId;
    gs_creatingEntityId = info.uid;

    // Take an extra reference on info.nameClass's shared buffer for the duration of this call
    // (released just before returning, on every path).
    dsTSTRING<char> nameClassRef;
    nameClassRef.pBuffer = info.nameClass.pBuffer;
    ++nameClassRef.pBuffer->refCount;
    const char *nameClassStr = nameClassRef.CStr();

    dsTSTRING<char> nameClassCopy;
    nameClassCopy.pBuffer = nullptr;
    nameClassCopy.UnsafeInit(nameClassStr, -1, 0);

    entENTITY *ent = iaFamily->CreateIA(nameClassCopy);

    dsTSTRING_BUF_HEADER<char> *nameClassCopyBuffer = nameClassCopy.pBuffer;
    if (--nameClassCopyBuffer->refCount == 0)
        dlFree(nameClassCopyBuffer);

    entENTITY *result;

    if (!ent)
    {
        // Deviation from decompiler: `pInst->name` is read here with no null guard on `pInst`
        // itself, matching the disasm exactly -- this error path assumes a non-null instance.
        const char *instName = pInst->name ? pInst->name : empty_string;
        _apLog("~Entity,Error~Cannot find entity class %s (instName: %s)", nameClassStr, instName);
        gsScenePtr->DestroyInst(pInst);
        result = nullptr;
    }
    else
    {
        if (pInst)
            pInst->SetNameClass(nameClassStr);

        if (info.pCD)
            ent->pDomSpawn = info.pCD->pDomSpawn;

        if (pInst)
        {
            ent->pInst = pInst;
            pInst->pEnt = ent;

            // Prefer the instance's own section if it (or its parent hierarchy) has any keys;
            // otherwise fall back to the template's section.
            psSECTION section;
            if (((const psSECTION_INTERFACE &)pInst->ps).IsEmptyHier())
                section = pInst->pTpl->ps;
            else
                section = pInst->ps;

            // Function-local static: lazily built once ("Unsharing ps for "), reused every call.
            static bool hintErrInited = false;
            static dsTSTRING<char> hintErr;
            if (!hintErrInited)
            {
                hintErrInited = true;
                hintErr.pBuffer = nullptr;
                hintErr.UnsafeInit("Unsharing ps for ", -1, 0);
            }

            const char *instName = pInst->name ? pInst->name : empty_string;
            dsTSTRING<char> hintErrConcat = operator+(hintErr, instName);

            ent->UnshareDescIfNeeded(section, hintErrConcat);

            dsTSTRING_BUF_HEADER<char> *hintErrConcatBuffer = hintErrConcat.pBuffer;
            if (--hintErrConcatBuffer->refCount == 0)
                dlFree(hintErrConcatBuffer);
        }

        if (ent->Register(info))
        {
            if (pInst)
                pInst->state2 |= 0x200000u;

            dscBRAND *pBrand = Desc(ent)->pBrand;

            bool isPlayerClass;
            {
                dsTSTRING<char> playerStr;
                playerStr.pBuffer = nullptr;
                playerStr.UnsafeInit("player", -1, 0);

                if (dscDESC_FAMILY::IsDerivedFrom(pBrand, playerStr))
                {
                    isPlayerClass = true;
                }
                else
                {
                    const char *instClassName = pInst->nameClass ? pInst->nameClass : empty_string;
                    isPlayerClass = (strcmp(instClassName, "player") == 0);
                }

                dsTSTRING_BUF_HEADER<char> *playerBuffer = playerStr.pBuffer;
                if (--playerBuffer->refCount == 0)
                    dlFree(playerBuffer);
            }

            if (isPlayerClass)
            {
                ent->cnt = &cnt_plr;
            }
            else
            {
                dsTSTRING<char> pjlStr;
                pjlStr.pBuffer = nullptr;
                pjlStr.UnsafeInit("pjl_base", -1, 0);
                bool isPjlBase = dscDESC_FAMILY::IsDerivedFrom(pBrand, pjlStr);

                dsTSTRING_BUF_HEADER<char> *pjlBuffer = pjlStr.pBuffer;
                if (--pjlBuffer->refCount == 0)
                    dlFree(pjlBuffer);

                if (isPjlBase)
                {
                    ent->cnt = &cnt_pjl;
                }
                else
                {
                    dsTSTRING<char> wpnStr;
                    wpnStr.pBuffer = nullptr;
                    wpnStr.UnsafeInit("weapon", -1, 0);
                    bool isWeapon = dscDESC_FAMILY::IsDerivedFrom(pBrand, wpnStr);

                    dsTSTRING_BUF_HEADER<char> *wpnBuffer = wpnStr.pBuffer;
                    if (--wpnBuffer->refCount == 0)
                        dlFree(wpnBuffer);

                    if (isWeapon)
                        ent->cnt = &cnt_wpn;
                }
            }

            result = ent;
        }
        else
        {
            msgSystem.SendMsg(ent, 2, nullptr, nullptr);
            if ((ent->stateAddr.state & 0x80) != 0)
                msgSystem.RemoveSubscriber(ent, 0);

            // Deviation from decompiler: it rendered this as an explicit vtable-slot indirect
            // call (a `dtr_entENTITY` field dereferenced off `ent`'s vtable, called with a
            // trailing flag=1 argument) -- the standard MSVC deleting-destructor idiom, i.e.
            // exactly `delete ent;` in source.
            delete ent;

            result = nullptr;
        }
    }

    dsTSTRING_BUF_HEADER<char> *nameClassRefBuffer = nameClassRef.pBuffer;
    if (--nameClassRefBuffer->refCount == 0)
        dlFree(nameClassRefBuffer);

    gs_creatingEntityId = savedCreatingEntityId;
    return result;
}
