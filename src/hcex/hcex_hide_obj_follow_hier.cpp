/* hcex_hide_obj_follow_hier @0x823B2BE8 — recursively hide/unhide a Blam object and its hierarchy for
 * cinematic capture. Sets HCEX_OBJ's hidden flag (bit 30) and the low bit of the animation instance's
 * state to reflect `isHide`. The child link is always followed (a hidden parent always cascades its
 * hidden state to its children); the sibling ("next") link is only followed when `isHideNext` is set,
 * and every recursive call forces isHideNext=1 so hiding one link of a sibling chain hides the rest of
 * the chain too. */

#include "../headers/hcex/hcex_cine_tick_boundary.h"

void hcex_hide_obj_follow_hier(int id, int isHide, int isHideNext)
{
    HCEX_OBJ *obj = hcex_obj_get(id);
    if ( !obj )
        return;

    if ( isHide )
        HCEX_OBJ_FLAGS(obj) |= HCEX_OBJ_FLAG_IS_HIDDEN;
    else
        HCEX_OBJ_FLAGS(obj) &= ~HCEX_OBJ_FLAG_IS_HIDDEN;

    obj->pInst->state = (obj->pInst->state & ~1) | (isHide ? 1 : 0);

    int idChild, idNext;
    hcex_obj_get_child_and_next(id, &idChild, &idNext);

    if ( isHideNext && idNext != -1 )
        hcex_hide_obj_follow_hier(idNext, isHide, 1);
    if ( idChild != -1 )
        hcex_hide_obj_follow_hier(idChild, isHide, 1);
}
