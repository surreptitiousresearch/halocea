/* hcex_update_pelican_effects @0x823D99E0 — drive a vehicle's thruster/effect animation state from the Blam
 * side. Looks up the HCEX object for vehicle_index; if it has an entity, it sets the entity's sml state to
 * "<src_obj>_start" (also positioning the named marker nodes from point/vector) when starting, or
 * "<src_obj>_stop" when stopping. No-op if the object, its instance, or its entity is missing.
 *
 * The decompiler tail of phantom int args (a6..a14) is a param-shuffle artifact; the real prototype is
 * (vehicle_index, src_obj, point, vector, start). entENTITY.stateSml lives at byte offset 0x24 (verified via
 * disasm; matches the hcex_create_effect convention). ent/anim model + sml are ws-engine boundaries. */

#include "../headers/hcex/hcex_create_effect_boundary.h"   /* HCEX_OBJ, animINST, entENTITY, sml_STATE_Set, hcex_obj_get, hcex_prepare_locations, sprintf_0, dsTSTRING_flat, dlFree */

extern char *hcex_conv_name(char *out, int max_len);

extern "C" void hcex_update_pelican_effects(int vehicle_index, const char *src_obj, hcex_float3 *point,
        hcex_float3 *vector, int start)
{
    HCEX_OBJ *obj = hcex_obj_get(vehicle_index);
    if ( !obj )
        return;

    animINST *inst = obj->pInst;
    if ( !inst || !inst->pEnt )
        return;

    char state_name[176];
    if ( start )
    {
        const char *name_ptr = src_obj;
        sprintf_0(state_name, "%s_start", src_obj);
        hcex_prepare_locations(obj->pInst, point, vector, &name_ptr, 1);
    }
    else
    {
        sprintf_0(state_name, "%s_stop", src_obj);
    }
    hcex_conv_name(state_name, 128);

    dsTSTRING_flat state;
    state.pBuffer = 0;
    dsTSTRING_UnsafeInit(&state, state_name, -1, 0);
    /* +0x24 == iaIACTOR::stateSml (sml::STATE, 12B); flat sml_STATE spelling is this file's
     * C-linkage view of the same type */
    sml_STATE_Set((sml_STATE *)&obj->pInst->pEnt->stateSml, &state);

    if ( --state.pBuffer->refCount == 0 )
        dlFree(state.pBuffer);
}
