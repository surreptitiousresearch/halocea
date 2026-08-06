#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ent/entCREATE_INFO.h"
#include "../../headers/ws/scn/scnSCENE.h"
#include "../../headers/ws/gs/gsLVL_SYSTEM.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/entCREATE_DATA.h"
#include <string.h>

extern "C" void dlFree(void *ptr);

// Declared in entCreate.cpp (the animINST-overload, lowest address). Same free-function family.
entENTITY *entCreate(animINST *pInst, const entCREATE_INFO &info);

extern gsLVL_SYSTEM *gsSysLevel;

// entCreate(scnSCENE*, entCREATE_INFO&) — 0x82536A48
//
// Convenience glue between the 5-arg scnSCENE overload and the main entCreate: resolves
// `info.nameTpl` to a live animTPL (unless it is the "-" sentinel, meaning "no scene instance --
// call the base entCreate directly with a null animINST"), defaults `info.nameClass` from the
// template's own class name when the caller left it empty, allocates the scnSCENE instance, then
// forwards to entCreate(animINST*, const entCREATE_INFO&).
//
// CAVEAT: dsTSTRING<char>::operator= is compiler-inlined at the nameClass-adoption point (no
// `bl`) -- reconstructed as the equivalent manual release/adopt/bump sequence (see
// entCREATE_INFO::entCREATE_INFO for the same convention).
entENTITY *entCreate(scnSCENE *pScn, entCREATE_INFO *info)
{
    animINST *inst = nullptr;

    if (strcmp(info->nameTpl.CStr(), "-") != 0)
    {
        animTPL *tpl = gsSysLevel->FindTplName(info->nameTpl.CStr(), nullptr, 0);
        if (!tpl)
            return nullptr;

        if (strcmp(info->nameClass.CStr(), "") != 0)
        {
            // Deviation from decompiler: it rendered this call with 5 extra phantom trailing
            // args (fabricated by the same float-slot-skip ABI mis-shift documented in
            // scnSCENE.h's CreateInst note) -- disasm confirms only (this, pTpl, pCD) are ever
            // loaded before the branch.
            inst = pScn->CreateInst(tpl, (animCREATE_DATA *)info->pCD);
        }
        else if (tpl->nameClass[0])
        {
            // info.nameClass was left empty: adopt the template's own default class name.
            dsTSTRING<char> newNameClass;
            newNameClass.pBuffer = nullptr;
            newNameClass.UnsafeInit(tpl->nameClass, -1, 0);

            dsTSTRING_BUF_HEADER<char> *oldNameClassBuffer = info->nameClass.pBuffer;
            if (--oldNameClassBuffer->refCount == 0)
                dlFree(oldNameClassBuffer);
            info->nameClass.pBuffer = newNameClass.pBuffer;
            ++info->nameClass.pBuffer->refCount;

            dsTSTRING_BUF_HEADER<char> *tempBuffer = newNameClass.pBuffer;
            if (--tempBuffer->refCount == 0)
                dlFree(tempBuffer);

            inst = pScn->CreateInst(tpl, (animCREATE_DATA *)info->pCD);
        }
        else
        {
            // Neither the caller nor the template supplied a class name -- unconditional
            // strong-assert-crash (decompiler's `"empty class name"` operand is just the
            // always-true literal used both as the crash gate and the logged message).
            if (!IGNORE_STRONG_ASSERT)
                // Crash is a non-static member (this passed null by the original); call on a null instance
                ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
                    "!\"empty class name\"",
                    "D:\\Projects\\code\\common\\src.sys\\gm_sys\\entity\\entity.cpp",
                    675,
                    empty_string);
            return nullptr;
        }

        if (!inst)
            return nullptr;
    }

    if (inst)
        inst->SetNameTpl(info->nameTpl.CStr());

    entENTITY *ent = entCreate(inst, *info);

    if (inst)
        inst->PostApplyCreateData((animCREATE_DATA *)info->pCD);

    return ent ? ent : nullptr;
}
