#include "scnSCENE.h"
#include "../ds/dsBIT_ARRAY_IMPL.h"
#include "../ps/psSECTION_INTERFACE.h"
#include "../obj/objOBJ.h" // full objOBJ type for MTL_DESC_DB::SelectMaterialsByInstAffixes calls

// material-descriptor database — other-subsystem boundary, not reversed here.
struct MTL_DESC_DB {
    // ?SelectMaterialsByInstAffixes@MTL_DESC_DB@@QAAXPAVobjOBJ@@ABV?$dsTSTRING@D@@@Z — re-resolve
    // material selection for `pObj` using the instance's affix string.
    void SelectMaterialsByInstAffixes(objOBJ *pObj, const dsTSTRING<char> &affixStr); // boundary
};
extern MTL_DESC_DB *mtlManager; // boundary — ?mtlManager@@3PAVMTL_DESC_DB@@A

// ?CreateInst@scnSCENE@@QAAPAVanimINST@@PAVanimTPL@@PAVanimCREATE_DATA@@@Z  (0x82540DD0) —
// allocate an instance from `pTpl` (or `pTplDummy` when null) via AllocInst (unattached),
// duplicate the template's model-object tree if it has one (destroying the fresh instance and
// bailing on a duplicate failure or if the template's root object is already bound to an
// instance), apply names/create-data/section overrides, merge the template's state flags into the
// instance (clearing bit 0x200 back off if the template set it), invalidate render/collision
// state, then splice the instance onto the live list and mark it attached.
//
// NOTE: like AllocInst/AddJustCreatedInstances, the decompiler fabricated 5 extra trailing int
// parameters (a4..a8). Disasm (0x82540dd0-0x82540fbc) confirms only r3 (this), r4 (pTpl) and r5
// (pCD) are ever read — the real prototype takes (animTPL*, animCREATE_DATA*).
animINST *scnSCENE::CreateInst(animTPL *pTpl, animCREATE_DATA *pCD)
{
    animTPL *tpl = pTpl ? pTpl : this->pTplDummy;

    objOBJ *tplObj = tpl->pObj;
    if (tplObj && tplObj->pInst)
        return nullptr;

    animINST *inst = this->AllocInst(tpl, 0);
    if (!inst)
        return nullptr;

    if (tplObj)
    {
        objOBJ *dup = tplObj->Duplicate(nullptr);
        inst->pObj = dup;
        if (!dup)
        {
            this->DestroyInst(inst);
            return nullptr;
        }
    }

    inst->stateVis |= 6u;
    inst->SetNameClass(tpl->nameClass);
    inst->SetNameTpl(tpl->name);
    inst->pTpl = tpl;
    inst->SetObjInstRef();
    inst->FillLinearList();
    inst->FillObjRendList(false);
    inst->idAnimCdt = -1;
    inst->ApplyCreateData(pCD);

    if (inst->affixes.str.pBuffer->strLen)
        mtlManager->SelectMaterialsByInstAffixes(inst->pObj, inst->affixes.str);

    if (!reinterpret_cast<psSECTION_INTERFACE *>(&tpl->ps)->IsEmptyHier())
    {
        // Reparent inst->ps under an empty body with both the template's section and the
        // instance's own prior section as parents (in that order).
        psSECTION priorInstPs(inst->ps);
        reinterpret_cast<psSECTION_INTERFACE *>(&inst->ps)->CreateEmpty();
        reinterpret_cast<psSECTION_INTERFACE *>(&inst->ps)->AddParent(tpl->ps);
        reinterpret_cast<psSECTION_INTERFACE *>(&inst->ps)->AddParent(priorInstPs);
    }

    int mergedState = inst->state | tpl->state;
    inst->state = mergedState;
    if (tpl->state & 0x200)
        inst->state = mergedState & ~0x200u; // template's own bit9 not inherited by the instance (bits unnamed in DB)

    inst->SetIGNORE_CDT_ALWAYS();
    inst->Invalidate(0x801, 1);

    inst->pScn = this;
    animINST *firstInst = this->pFirstInst;
    if (firstInst)
    {
        inst->next = firstInst;
        this->pFirstInst->prev = inst;
    }
    this->pFirstInst = inst;
    dsBIT_ARRAY_IMPL::SetYes(inst->id, this->isUsedInstState.array, 2500);

    return inst;
}
