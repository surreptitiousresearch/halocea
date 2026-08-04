/* hcex_obj_set_state @0x823DF5A8 — set the ws-engine animation-state name on a tracked Blam object's entity.
 * Looks the object up in hcexObjects; if it has an animation instance with an entity, pushes the state name
 * onto that entity's sml state. When the object is the weapon a local player is currently holding
 * (hcex_fp_weapon_plr_index >= 0), the same state is also applied to every first-person view model in
 * fpModels. Each state string is a temporary ref-counted dsTSTRING_flat, released after use.
 *
 * The decompiler rendered the sml::STATE::Set / dsTSTRING_flat thiscalls with spurious extra args; the real
 * prototype takes only the object id and state name. ent/anim/sml types are ws-engine boundaries. */

#include "../headers/hcex/hcex_obj_state_boundary.h"

extern "C" void hcex_obj_set_state(int objId, const char *stateName)
{
    HCEX_OBJ *obj = hcex_obj_get(objId);
    if ( !obj )
        return;

    animINST *inst = obj->pInst;
    if ( inst && inst->pEnt )
    {
        dsTSTRING_flat stateStr;
        stateStr.pBuffer = 0;
        dsTSTRING_UnsafeInit(&stateStr, stateName, -1, 0);
        sml_STATE_Set(&obj->pInst->pEnt->stateSml, &stateStr);

        dsTSTRING_BUF_HEADER<char> *buf = stateStr.pBuffer;
        if ( --buf->refCount == 0 )
            dlFree(buf);
    }

    if ( hcex_fp_weapon_plr_index(objId) >= 0 )
    {
        int i;
        for ( i = 0; i < fpModels.nElem; ++i )
        {
            dsTSTRING_flat stateStr;
            stateStr.pBuffer = 0;
            dsTSTRING_UnsafeInit(&stateStr, stateName, -1, 0);

            FP_MODEL *fp = dsVECTOR_FP_MODEL_index(&fpModels, i);
            sml_STATE_Set(&fp->pInst->pEnt->stateSml, &stateStr);

            dsTSTRING_BUF_HEADER<char> *buf = stateStr.pBuffer;
            if ( --buf->refCount == 0 )
                dlFree(buf);
        }
    }
}
