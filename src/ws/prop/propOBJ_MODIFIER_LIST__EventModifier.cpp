/* ?EventModifier@propOBJ_MODIFIER_LIST@@QAAXABV?$dsTSTRING@D@@0ABV?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@@Z @0x826D0680 */
#include "../../headers/ws/prop/propOBJ_MODIFIER_LIST.h"
#include "../../headers/ws/prop/propOBJ_MODIFIER_LIST_DESC.h"
#include "../../headers/ws/ds/dsCMP.h"

// Desc<propOBJ_MODIFIER_LIST>(const propOBJ_MODIFIER_LIST*) — 0x826D3838-family free template
// helper that returns the live object's descriptor (this->spDesc.pointee, downcast to the
// concrete DESC type). Same-family utility, not reversed in this batch. boundary.
extern propOBJ_MODIFIER_LIST_DESC *Desc(const propOBJ_MODIFIER_LIST *self);

// 0x826D0680
// CAVEAT: the decompiler's `v13 = v11;` store (var_1C -> var_10) is immediately overwritten by
// the very next store (*element -> var_10) before var_10 is read; disassembly confirms it is a
// dead store with no effect on behavior, so it is omitted below.
void propOBJ_MODIFIER_LIST::EventModifier(const dsTSTRING<char> &name, const dsTSTRING<char> &event,
                                           const ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> &params)
{
    // Intern `name` as a lookup key (existOnly=false: reuse or create the pooled entry).
    dsSTRID key(name, false);

    propOBJ_MODIFIER_LIST_DESC *desc = Desc(this);
    dsCMP defaultCmp = {};

    int modifierIndex = desc->listModifiers.Find<dsCMP, dsSTRID>(key, defaultCmp, 0);
    if (modifierIndex != -1)
    {
        dsSHARED_PTR<gsOBJ_MODIFIER_BASE, 0, Deleter<gsOBJ_MODIFIER_BASE> > &liveModifier =
            this->listModifiers[modifierIndex];

        liveModifier.pointee->OnEvent(&event, params.m_pData->m_pT);
    }
}
