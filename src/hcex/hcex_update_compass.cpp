/* hcex_update_compass @0x823E4E70 — push the assault-rifle compass heading into the recreated Flash HUD.
 * Resolves the ws-engine gfx modifier for the given first-person model / local player, builds a one-entry
 * ref-counted parameter list { "angle": compass_magnitude * -360 } (the [0,1] needle position converted to a
 * negated degree rotation for the Flash gauge), and fires the "asSetCompass" ActionScript event in the "gfx"
 * category. Returns the input compass_magnitude, or 0 when the model has no modifier. Called from
 * hcex_update_weapon_state.
 *
 * DEVIATION: the third argument is a float (compass_magnitude); the decompiler widened it to double via the
 * FPR-arg convention. The "angle" value is computed in float then stored into a double parameter slot
 * (Add<double>), matching the disassembly. The ws modifier / ref-type / parameter-list types are extern
 * boundaries. */

#include "../headers/hcex/hcex_modifier_boundary.h"

extern "C" float hcex_update_compass(int modelId, __int16 local_player_index, float compass_magnitude)
{
    propOBJ_MODIFIER_LIST *modifier = GetModifier(modelId, local_player_index);
    if ( !modifier )
        return 0.0f;

    REF_TYPE_DATA_param_list *data = (REF_TYPE_DATA_param_list *)ds_operator_new_dbg(
        sizeof(REF_TYPE_DATA_param_list),
        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_ref_type.h", 0x51);
    if ( data )
        REF_TYPE_DATA_param_list_ctor(data);

    REF_TYPE_param_list params;
    params.m_pData = data;

    double angle = (float)(compass_magnitude * -360.0f);
    dsVECTOR_PARAM_LIST *list = data->m_pT;

    dsSTRID angleKey;
    dsSTRID_ctor(&angleKey, "angle", false);
    dsVECTOR_PARAM_LIST_Add_double(list, angleKey, &angle);

    dsTSTRING_flat event;
    event.pBuffer = 0;
    dsTSTRING_UnsafeInit(&event, "asSetCompass", -1, 0);
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
    return compass_magnitude;
}
