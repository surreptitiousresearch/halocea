#include "scnSCENE.h"
#include "../ds/dsBIT_ARRAY_IMPL.h"
#include "../ds/dsTSTRING.h"
#include "../ds/ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — CRT/Saber heap free

// ?scnDumpInstList@@YAXPAVscnSCENE@@ABV?$dsTSTRING@D@@@Z — free function, dumps the live instance
// list to a CSV file for diagnostics. Called only on the (should-be-unreachable) free-list-empty
// path below.
extern void scnDumpInstList(scnSCENE *pScn, const dsTSTRING<char> *nameFile); // matches scnDumpInstList.cpp definition

// ?AllocInst@scnSCENE@@QAAPAVanimINST@@PAVanimTPL@@H@Z  (0x82540C90) — pop the head of the free
// list and initialise it as a fresh instance bound to `pTpl`: identity transform, unit scale, sign
// stamped, no physics, LOD not loaded. When `isAttach` is set, also splices it onto the head of
// the live list and marks its `isUsedInstState` bit. When the free list is exhausted (pool full),
// dumps the live list to "scn_inst.csv" and strong-assert-crashes (returns null if asserts are
// disabled).
//
// NOTE: like AddJustCreatedInstances, the decompiler fabricated 6 extra trailing int parameters
// (a4..a8). Disasm (0x82540c90-0x82540dcc) confirms only r3 (this), r4 (pTpl) and r5 (isAttach)
// are ever read — the real prototype takes (animTPL*, int). AllocInst calls
// AddJustCreatedInstances with exactly 2 real args (this, id); the extra decompiler args on that
// call site were phantom forwards of AllocInst's own phantom params.
animINST *scnSCENE::AllocInst(animTPL *pTpl, int isAttach)
{
    animINST *inst = this->pFirstFree;
    if (!inst)
    {
        dsTSTRING<char> nameFile;
        nameFile.pBuffer = nullptr;
        nameFile.UnsafeInit("scn_inst.csv", -1, 0);
        scnDumpInstList(this, &nameFile);
        if (nameFile.pBuffer->refCount-- == 1)
            dlFree(nameFile.pBuffer);

        if (!IGNORE_STRONG_ASSERT)
        {
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "0",
                "D:\\Projects\\code\\common\\src.sys\\objects\\Scene.cpp",
                872,
                empty_string);
        }
        return nullptr;
    }

    this->pFirstFree = inst->next;
    inst->next = nullptr;
    this->AddJustCreatedInstances(inst->id);
    inst->pTpl = pTpl;
    inst->sign = 1095323219; // 'SNIA' — allocation signature stamp
    inst->matrL2W.Identity();
    inst->pPhysChar = nullptr;
    inst->idxLoaded = -1;
    inst->scale.z = 1.0f;
    inst->scale.y = 1.0f;
    inst->scale.x = 1.0f;

    if (isAttach)
    {
        inst->pScn = this;
        animINST *firstInst = this->pFirstInst;
        if (firstInst)
        {
            inst->next = firstInst;
            this->pFirstInst->prev = inst;
        }
        this->pFirstInst = inst;
        dsBIT_ARRAY_IMPL::SetYes(inst->id, this->isUsedInstState.array, 2500);
    }

    return inst;
}
