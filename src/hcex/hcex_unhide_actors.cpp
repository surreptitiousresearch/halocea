/* hcex_unhide_actors @0x823B6EE0 — undo the per-frame cinematic-actor hiding done by hcex_cine_update:
 * for every tracked HCEX_OBJ whose Blam id is in cineHiddenHcexObjKeys (the set of ids hidden this
 * cinematic), un-hide it and its hierarchy, then clear both the hidden-key set and the hidden-template-
 * name list so the next cinematic starts with a clean slate. Called from hcex_stop_cine.
 *
 * The bounds check the decompiler inlines from dsVECTOR<int,8>::operator[] (ds_vector.hpp:567) is
 * reproduced faithfully via STRONG_ASSERT2_asserd_int, matching the strong-assert idiom already
 * used elsewhere in this bridge layer (e.g. hcex_on_play_sound). */

#include "../headers/hcex/hcex_cine_tick_boundary.h"

void hcex_unhide_actors(void)
{
    int idx = 1;
    for ( HCEX_OBJ *obj = hcex_obj_get_by_idx(0); obj; obj = hcex_obj_get_by_idx(idx++) )
    {
        int nElem = cineHiddenHcexObjKeys.nElem;
        for ( int i = 0; i < nElem; ++i )
        {
            if ( !IGNORE_STRONG_ASSERT && (i < 0 || i >= nElem) )
                STRONG_ASSERT2_asserd_int("idx >= 0 && idx < nElem",
                                           "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 567,
                                           "idx", &i, "nElem", &cineHiddenHcexObjKeys.nElem);

            if ( obj->id == cineHiddenHcexObjKeys.pData[i] )
            {
                hcex_hide_obj_follow_hier(obj->id, 0, 0);
                break;
            }
        }
    }

    dsVECTOR_tstring_Clear(&cineHiddenActorsTplNames);
    cineHiddenHcexObjKeys.nElem = 0;
}
