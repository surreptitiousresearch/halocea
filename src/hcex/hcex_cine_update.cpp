/* hcex_cine_update @0x823B91E8 — per-frame HCEX cinematic tick, run only while gsMsgIsMode(0x10) (the
 * cinematic game-mode bit) is set. Two independent passes:
 *
 * 1. For every ws-engine entity the Anitec system owns for the active cinematic (gsSysAnitec->ownedActors),
 *    resolve its Blam object id via the hcexObjectsCine name->id cache (populated lazily on a cache miss by
 *    a linear scan of every tracked HCEX_OBJ's scenario name), then hide/unhide it per
 *    dbg_hcex_HideCineActors (no sibling cascade — isHideNext=0).
 * 2. For every tracked HCEX_OBJ whose animation template name is in cineHiddenActorsTplNames (the
 *    "HideCineActorsByTplName" debug list, see hcex_cine_init), hide/unhide it per
 *    dbg_hcex_HideCineActors and remember its id in cineHiddenHcexObjKeys so hcex_unhide_actors can
 *    restore it later.
 *
 * entENTITY::GetName (a virtual call inlined at the ownedActors[i] site in the decompile) is flattened
 * to a named boundary call, per the project's vtable-indirection convention. */

#include "../headers/hcex/hcex_cine_tick_boundary.h"

extern "C" int strcmp(const char *a, const char *b);

void hcex_cine_update(void)
{
    gsAnitecElapsedTicsExternal = hcex_get_elapsed_tics();

    if ( !gsMsgIsMode(0x10u) )
        return;

    for ( int i = 0; i < gsSysAnitec->ownedActors.nElem; ++i )
    {
        entENTITY *actor = *dsVECTOR_ent_index(&gsSysAnitec->ownedActors, i);

        dsTSTRING_flat actorName;
        actorName.pBuffer = 0;
        entENTITY_GetName(actor, &actorName);

        dsCMP cmp;
        int cached = dsVECTOR_cine_pair_FindSorted(&hcexObjectsCine, &actorName, &cmp);

        if ( cached == -1 )
        {
            HCEX_OBJ *found = 0;
            int idx = 1;
            for ( HCEX_OBJ *obj = hcex_obj_get_by_idx(0); obj; obj = hcex_obj_get_by_idx(idx++) )
            {
                const char *name = hcex_obj_get_name(obj->id);
                if ( name && strcmp(actorName.pBuffer->str, name) == 0 )
                {
                    found = obj;
                    break;
                }
            }

            if ( found )
            {
                hcex_cine_pair entry;
                entry.key = actorName;
                entry.val = found->id;
                ++actorName.pBuffer->refCount; /* the pair now shares this string's buffer too */
                cmp = dsCMP();
                cached = dsVECTOR_cine_pair_InsertSorted(&hcexObjectsCine, &entry, &cmp, INS_DUP_IGNORE);

                if ( cached != -1 )
                    hcex_hide_obj_follow_hier(dsVECTOR_cine_pair_index(&hcexObjectsCine, cached)->val,
                                               dbg_hcex_HideCineActors.value, 0);
            }
        }
        else
        {
            hcex_hide_obj_follow_hier(dsVECTOR_cine_pair_index(&hcexObjectsCine, cached)->val,
                                       dbg_hcex_HideCineActors.value, 0);
        }

        if ( --actorName.pBuffer->refCount == 0 )
            dlFree(actorName.pBuffer);
    }

    int idx = 1;
    for ( HCEX_OBJ *obj = hcex_obj_get_by_idx(0); obj; obj = hcex_obj_get_by_idx(idx++) )
    {
        animINST *inst = obj->pInst;
        if ( !inst )
            continue;

        const char *tplName = inst->nameTpl ? inst->nameTpl : empty_string;

        for ( int i = 0; i < cineHiddenActorsTplNames.nElem; ++i )
        {
            const dsTSTRING_flat *hiddenName = (const dsTSTRING_flat *)cineHiddenActorsTplNames.pData + i;
            if ( strcmp(hiddenName->pBuffer->str, tplName) == 0 )
            {
                hcex_hide_obj_follow_hier(obj->id, dbg_hcex_HideCineActors.value, 0);

                dsCMP cmp;
                dsVECTOR_int_InsertSorted(&cineHiddenHcexObjKeys, &obj->id, &cmp, INS_DUP_IGNORE);
                break;
            }
        }
    }
}
