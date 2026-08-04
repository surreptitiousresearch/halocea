/* hcex_light_sync @0x823D3178 — push one bridged Blam flashlight light's current placement/radius into
 * its ws-engine entity for this frame. Resolves the light's world matrix and effective radius via
 * hcex_get_light_params; if the entity behind the weak handle is gone, does nothing. A near-zero radius
 * hides the light entity (unless already hidden); otherwise it's shown, its instance transform updated,
 * and — for a local player's own attached light (plr_idx <= 1) — its first-person camera visibility is
 * synced via first_person_weapon_visible. Finally rescales the radius on any attached entSFX_LIGHT.
 *
 * deviation: hcex_get_light_params' radius out-param decompiles as a 3-float stack array (v15[3]) purely
 * from local stack-slot overlap with the following hcex_matr4x3; only v15[0] is ever read, matching the
 * real (scalar float*) signature already reversed in hcex_get_light_params.c. Reproduced here as a plain
 * float. */

#include "../headers/hcex/hcex_light_sync_boundary.h"

void hcex_light_sync(HCEX_LIGHT *light)
{
    hcex_matr4x3 halo_matr;
    float        radius;
    int          plr_idx;

    hcex_get_light_params(light->id, &halo_matr, &radius, &plr_idx);

    ds_WEAK_PTR_HANDLE *handle = light->ent.pHandle;
    entENTITY *entity = handle ? handle->pPtr : 0;
    if ( !entity )
        return;

    bool was_hidden = (entity->stateEnt.val & ENT_ST_HIDDEN_BIT) != 0;

    if ( radius < 0.000001f )
    {
        if ( !was_hidden )
            entENTITY_implHide(entity);
        return;
    }

    if ( was_hidden )
        entENTITY_implShow(entity);

    handle = light->ent.pHandle;
    entity = handle ? handle->pPtr : 0;
    animINST *inst = entity->pInst;

    m3dMATR ws_matr;
    hcex_make_inst_matr(&halo_matr, &ws_matr);
    inst->Transform(&ws_matr, 0);

    if ( plr_idx <= 1 )
    {
        int visible_to_fp_cam = first_person_weapon_visible((short)plr_idx);
        if ( visible_to_fp_cam )
            entENTITY_ShowToCamera(entity, plr_idx);
        else
            entENTITY_HideFromCamera(entity, plr_idx);
    }

    /* deviation: the original expresses this as `if (plr_idx) goto LABEL_24;` jumping into the middle
     * of the following if-block; flattened here into the equivalent pair of independent conditions. */
    if ( plr_idx == 0 && (inst->state2 & ANIMINST_STATE2_BIT8) == 0 )
        entENTITY_HideFromCamera(entity, 1);
    if ( plr_idx == 1 && (inst->state2 & ANIMINST_STATE2_BIT7) == 0 )
        entENTITY_HideFromCamera(entity, 0);

    int num_sfx = entity->GetNumSfx();
    for ( int i = 0; i < num_sfx; ++i )
    {
        entSFX *sfx = entENTITY_GetSfx(entity, i);
        if ( entSFX_IsA_LIGHT(sfx) )
            entSFX_LIGHT_SetRadScale((entSFX_LIGHT *)sfx, radius);
    }
}
