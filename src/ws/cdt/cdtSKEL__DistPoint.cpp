#include "../../headers/ws/cdt/cdtSKEL.h"
#include "../../headers/ws/cdt/cdtBONE.h"
#include "../../headers/ws/cdt/cdtINFO.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/m3d/m3dOBB.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// 0x82B0BEC4  ?DistPoint@cdtSKEL@@QBAHPAVanimINST@@PBUm3dV@@MPAVcdtREFINE@@PAVcdtINFO@@@Z
// Closest-point query of `point` (within `radius`) against the per-bone collision volumes of
// `pInst`. Broad-phase rejects with the instance OBB, then tests every bone keeping the nearest
// hit in `pInfo`. Returns nonzero on a hit.
int cdtSKEL::DistPoint(animINST *pInst, const m3dV *point, float radius,
                       cdtREFINE *pRefine, cdtINFO *pInfo) const
{
    if ( !IGNORE_STRONG_ASSERT && !this )
        STRONG_ASSERT_DUMMY().Crash(
            nullptr, "this != 0",
            "D:\\Projects\\code\\common\\src.sys\\animation\\anim_cdt.cpp", 169,
            empty_string);

    // Broad phase: reject when `point` is farther than `radius` from the instance's OBB.
    // No outputs requested (vClosest/dist/faceNmb all null per disasm r6=r7=r8=0).
    m3dOBB *obb = pInst->GetOBB(0);
    int result = obb->DistPoint(point, radius, nullptr, nullptr, nullptr);
    if ( !result )
        return result;

    int   bestBone = 0;
    int   haveHit  = 0;
    int   boneCount = this->bones.nElem;
    if ( boneCount <= 0 )
        return haveHit;

    for ( int i = 0; i < boneCount; ++i )
    {
        cdtINFO boneInfo;
        cdtBONE *bone = this->bones[i];
        // Virtual dispatch through __vftable slot 6 (+24). `radius` is the current best distance
        // and is tightened on every accepted hit.
        if ( bone->DistPoint(pInst, point, radius, pRefine, &boneInfo) )
        {
            radius   = boneInfo.distMin;
            haveHit  = 1;
            bestBone = i;
            if ( pInfo )
            {
                // Copy the whole 9-word cdtINFO hit record from the bone into the caller's.
                *pInfo = boneInfo;
            }
        }
    }

    if ( !haveHit )
        return haveHit;

    pInfo->___u2.pInst = pInst;

    // Mark the hit as "instance/bone" and normalise the collision-response classification bits.
    unsigned int state = pInfo->state.state | 2u;
    pInfo->state.state = state;
    if ( (state & 0x100) != 0 )
    {
        pInfo->state.state = state & ~0x100u;
        state = (state & ~0x6100u) | 0x6000;
    }
    else
    {
        if ( (state & 0x1000) == 0 )
            return haveHit;
        unsigned int s2 = pInfo->state.state;
        pInfo->state.state = s2 & ~0x1000u;
        state = (s2 & ~0x3000u) | 0x2000;
    }
    pInfo->state.state = state;

    pInfo->___u3.id = this->bones[bestBone]->uid;
    return haveHit;
}
