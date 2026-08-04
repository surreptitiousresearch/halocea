/* hcex_update_heat @0x823E4D10 — push a plasma weapon's overheat level into the recreated Flash HUD.
 * Resolves the ws-engine gfx modifier for the given first-person model / local player, builds a one-entry
 * ref-counted parameter list { "level": (int)(heat_level * 100) } (heat as an integer percentage), and fires
 * the "asSetOverheat" ActionScript event in the "gfx" category. Returns the input heat_level, or -1 when the
 * model has no modifier. Called from hcex_update_weapon_state.
 *
 * DEVIATION: the third argument is a float (heat_level); the decompiler widened it to double via the
 * FPR-arg convention. Prototype matches the caller (hcex_update_weapon_state) which passes a float. The ws
 * modifier / ref-type / parameter-list types are extern boundaries. */

#include "../headers/hcex/hcex_modifier_boundary.h"

extern "C" float hcex_update_heat(int modelId, __int16 local_player_index, float heat_level)
{
    propOBJ_MODIFIER_LIST *modifier = GetModifier(modelId, local_player_index);
    if ( !modifier )
        return -1.0f;

    REF_TYPE_DATA_param_list *data = (REF_TYPE_DATA_param_list *)ds_operator_new_dbg(
        sizeof(REF_TYPE_DATA_param_list),
        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_ref_type.h", 0x51);
    if ( data )
        REF_TYPE_DATA_param_list_ctor(data);

    REF_TYPE_param_list params;
    params.m_pData = data;

    int level = (int)(heat_level * 100.0f);
    dsVECTOR_PARAM_LIST *list = data->m_pT;

    dsSTRID levelKey;
    dsSTRID_ctor(&levelKey, "level", false);
    dsVECTOR_PARAM_LIST_Add_int(list, levelKey, &level);

    dsTSTRING_flat event;
    event.pBuffer = 0;
    dsTSTRING_UnsafeInit(&event, "asSetOverheat", -1, 0);
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
    return heat_level;
}
