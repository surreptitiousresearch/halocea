/* hcex_update_ammo @0x823E4BD8 — push the assault-rifle magazine count into the recreated Flash HUD.
 * Resolves the ws-engine gfx modifier for the given first-person model / local player, builds a one-entry
 * ref-counted parameter list { "ammo": arifle_ammo }, and fires the "asSetAmmo" ActionScript event in the
 * "gfx" category on that modifier. Returns the ammo value that was pushed, or -1 when the model has no
 * modifier. Called from hcex_update_weapon_state.
 *
 * The decompiler inflated the thiscall with ~13 phantom int args and split the ref-list construction across
 * aliased temporaries; the real prototype is (modelId, local_player_index, arifle_ammo). The ws modifier /
 * ref-type / parameter-list types are extern boundaries. */

#include "../headers/hcex/hcex_modifier_boundary.h"

extern "C" int hcex_update_ammo(int modelId, __int16 local_player_index, int arifle_ammo)
{
    propOBJ_MODIFIER_LIST *modifier = GetModifier(modelId, local_player_index);
    if ( !modifier )
        return -1;

    REF_TYPE_DATA_param_list *data = (REF_TYPE_DATA_param_list *)ds_operator_new_dbg(
        sizeof(REF_TYPE_DATA_param_list),
        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_ref_type.h", 0x51);
    if ( data )
        REF_TYPE_DATA_param_list_ctor(data);

    REF_TYPE_param_list params;
    params.m_pData = data;
    dsVECTOR_PARAM_LIST *list = data->m_pT;

    dsSTRID ammoKey;
    dsSTRID_ctor(&ammoKey, "ammo", false);
    dsVECTOR_PARAM_LIST_Add_int(list, ammoKey, &arifle_ammo);

    dsTSTRING_flat event;
    event.pBuffer = 0;
    dsTSTRING_UnsafeInit(&event, "asSetAmmo", -1, 0);
    dsTSTRING_flat category;
    category.pBuffer = 0;
    dsTSTRING_UnsafeInit(&category, "gfx", -1, 0);

    propOBJ_MODIFIER_LIST_EventModifier(modifier, &category, &event, &params);

    dsTSTRING_BUF_HEADER<char> *catBuf = category.pBuffer;
    if ( --catBuf->refCount == 0 )
        dlFree(catBuf);
    dsTSTRING_BUF_HEADER<char> *evBuf = event.pBuffer;
    if ( --evBuf->refCount == 0 )
        dlFree(evBuf);

    REF_TYPE_param_list_dtor(&params);
    return arifle_ammo;
}
