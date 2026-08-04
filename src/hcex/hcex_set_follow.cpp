/* hcex_set_follow @0x823E3A20 — register pFollower as an entity that follows the HCEX object identified
 * by objFollowId. Looks up the tracked HCEX_OBJ, appends a weak handle to its followers list, and records
 * the follower's local offset transform relative to the followed object (so it can be re-applied each tick).
 *
 * The ws-engine math/container/animation templates (m3dMATR, dsVECTOR, ds::WEAK_PTR, animINST, entENTITY)
 * are boundary types — declared, not reversed. */

#include "../headers/hcex/hcex_follow_boundary.h"

extern void STRONG_ASSERT_DUMMY_Crash(void *a, const char *expr, const char *file, int line, const char *msg);
extern const char IGNORE_STRONG_ASSERT;
extern const char empty_string[];

void hcex_set_follow(entENTITY *pFollower, int objFollowId)
{
    HCEX_OBJ *obj = hcex_obj_get(objFollowId);

    if ( !IGNORE_STRONG_ASSERT && !obj )
        STRONG_ASSERT_DUMMY_Crash(0, "obj", "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 1979, empty_string);

    if ( obj->pInst )
    {
        ds_WEAK_PTR follower_handle;
        m3dMATR followed_l2w;
        m3dMATR follower_l2w;
        hcex_matr4x3 followed_obj_matr;
        m3dMATR followed_inverse;

        follower_handle.pHandle = 0;
        if ( pFollower )
        {
            follower_handle.pHandle = entENTITY_GetWeakHandle(pFollower);
            osLockedIncrement((volatile long *)&follower_handle.pHandle->refCount); // refCount int; API takes volatile long*
        }
        dsVECTOR_WEAKPTR_PushBack(&obj->followers, &follower_handle);
        ds_WEAK_PTR_dtor(&follower_handle);

        /* followed object's world transform, with HCEX's authored matrix folded in and scale stripped */
        animINST_GetMatrL2W(obj->pInst, &followed_l2w);
        hcex_get_obj_matr(objFollowId, &followed_obj_matr);
        hcex_make_inst_matr(&followed_obj_matr, &followed_l2w);
        m3dMATR_RemoveScale(&followed_l2w);
        m3dMATR_Invert(&followed_l2w, &followed_inverse);

        /* follower's world transform expressed in the followed object's space = the stored offset */
        animINST_GetMatrL2W(pFollower->pInst, &follower_l2w);
        m3dMATR_Transform(&follower_l2w, &followed_inverse, 2);
        dsVECTOR_MATR_PushBack(&obj->followersOfs, &follower_l2w);
    }
}
