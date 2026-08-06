// scrSCORCH_MANAGER::FindAround @0x82733080
//
// Overlap query used before adding a new scorch/decal of class `classID` centred at `pos` with the
// given `radius`. It walks the existing static and dynamic scorch marks whose footprint could
// intersect the query sphere and, for each same-class mark:
//   * if the new mark is fully contained by an existing one, returns true (caller skips adding);
//   * otherwise, if the existing mark is redundant with the new one, schedules it for removal.
// After scanning, the collected redundant marks are removed (dynamic then static). Returns false
// when the new mark should be added, true when it is already covered.
//
// The whole operation runs under the global multithread lock (g_syncMT). `radNoAround < 0` means
// "use radius". `overlappableOnly` restricts the scan to marks whose descriptor allows overlap.
//
// Signature per DB mangle: dsSTRID by value (a 4-byte interned id), pos by const reference, two
// floats, and the bool. The decompiler additionally invented two phantom parameters
// (filterParallax/a7) from register spills — they are not part of the real ABI.
//
// FIELD-OFFSET NOTES (DB types_members, authoritative):
//   scrSCORCH_MARK_BASE: desc@0, classID@4, radius@8, pos@0x18 (named via the header).
//   scrSCORCH_DESC     : full layout now DB-resolved in headers/ws/scorch/scrSCORCH_DESC.h
//                        (visibleAreaPart@0x44, canBeOverlapped@0x48, radNoAround@0xA0).
//   dynamic source obj : mark->pBatch->desc.pObjFollow (scrBATCH_DYN@0x164 desc + pObjFollow@0x24
//                        = decompiler's +0x188), a typed objOBJ* — stateProc@0x1C, pInst@0x14,
//                        matrLT@0x40 per types_members objOBJ.
#include "../../headers/ws/scorch/scrSCORCH_MANAGER.h"
#include "../../headers/ws/scorch/scrSCORCH_MARK_DYN.h"
#include "../../headers/ws/scorch/scrSCORCH_DESC.h"
#include "../../headers/ws/scorch/scrBATCH_DYN.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/ds/dsCONST_ARRAY.h"
#include "../../headers/ws/ds/dsPAIR.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/m3d/m3dBOX.h"
#include "../../headers/ws/m3d/m3dMATR.h"
#include "../../headers/animINST.h"

// ---- boundary externs (deeper than the 2-level descent budget) -------------------------------
struct osLOCK;
extern "C" osLOCK g_syncMT;
void osLOCK__Lock(osLOCK *self, void *, int);
void osLOCK_Unlock(osLOCK *self, void *, int);
extern float m3dDist_2(const m3dV *p1, const m3dV *p2); // 0x8265BC48

bool scrSCORCH_MANAGER::FindAround(dsSTRID classID, const m3dV &pos, float radius,
                                   float radNoAround, bool overlappableOnly)
{
    osLOCK__Lock(&g_syncMT, nullptr, 0);

    if (radNoAround < 0.0f)
        radNoAround = radius;

    // Redundant-mark removal accumulators (fixed 32-slot inline arrays).
    dsCONST_ARRAY<ds::VAL_LIST<scrSCORCH_MARK_DYN>::ITERATOR, 32> dynToRemove;
    dynToRemove.nEntry = 0;
    dsCONST_ARRAY<dsPAIR<int, scrVOX *>, 32> statToRemove;
    statToRemove.nEntry = 0;

    // Collect the (<=8) unique voxels covering the query sphere's AABB corners.
    m3dBOX queryBox;
    queryBox.bll.x = pos.x - radius;
    queryBox.bll.y = pos.y - radius;
    queryBox.bll.z = pos.z - radius;
    queryBox.fur.x = pos.x + radius;
    queryBox.fur.y = pos.y + radius;
    queryBox.fur.z = pos.z + radius;

    m3dV corners[8];
    queryBox.GetVertices(corners);

    scrVOX *uniqueVox[8];
    int nUnique = 0;
    for (int c = 0; c < 8; ++c)
    {
        int cell = this->voxGrid.GetPosLinearInd(&corners[c]);
        scrVOX *vox = &this->scrVoxels[cell]; // vector stores scrVOX by value
        int j = 0;
        for (; j < nUnique; ++j)
            if (uniqueVox[j] == vox)
                break;
        if (j == nUnique && nUnique < 8)
            uniqueVox[nUnique++] = vox;
    }

    // ---- static marks ---------------------------------------------------------------------------
    for (int vi = 0; vi < nUnique; ++vi)
    {
        scrVOX *vox = uniqueVox[vi];
        if (vox->scorchmarks.nElem <= 0)
            continue;

        for (int mi = 0; mi < vox->scorchmarks.nElem; ++mi)
        {
            scrSCORCH_MARK_STAT *mark = &vox->scorchmarks.pData[mi];
            scrSCORCH_DESC *desc = mark->desc.pointee;

            if (overlappableOnly && !desc->canBeOverlapped)
            {
                // Non-overlappable existing mark.
                float radiusEff = desc->visibleAreaPart * mark->radius;
                float dist2 = m3dDist_2(&mark->pos, &pos);
                if (dist2 < (radiusEff + radius) * (radiusEff + radius))
                {
                    if ((dist2 > (radius - radiusEff) * (radius - radiusEff) && radius < radiusEff * 2.0f)
                        || radius <= radiusEff)
                    {
                        osLOCK_Unlock(&g_syncMT, nullptr, 0);
                        return true; // new mark fully covered
                    }
                    if (statToRemove.nEntry < 32)
                    {
                        dsPAIR<int, scrVOX *> entry(mi, vox);
                        statToRemove.Add(entry);
                    }
                }
            }
            else if (classID.id == mark->classID.id) // same-class overlappable mark
            {
                float radiusEff = desc->radNoAround;
                if (radiusEff <= 0.0f)
                    radiusEff = desc->visibleAreaPart * mark->radius;
                float dist2 = m3dDist_2(&mark->pos, &pos);
                if (dist2 < (radiusEff + radNoAround) * (radiusEff + radNoAround))
                {
                    if (desc->radNoAround > 0.0f)
                        radiusEff = desc->visibleAreaPart * mark->radius;
                    if ((dist2 > (radius - radiusEff) * (radius - radiusEff) && radius < radiusEff * 2.0f)
                        || radius <= radiusEff)
                    {
                        osLOCK_Unlock(&g_syncMT, nullptr, 0);
                        return true;
                    }
                    if (statToRemove.nEntry < 32)
                    {
                        dsPAIR<int, scrVOX *> entry(mi, vox);
                        statToRemove.Add(entry);
                    }
                }
            }
        }
    }

    // ---- dynamic marks --------------------------------------------------------------------------
    for (int elemIdx = this->dynScorchList.usedHead; elemIdx != -1; )
    {
        ds::VAL_LIST<scrSCORCH_MARK_DYN>::RAW_NODE *node = &this->dynScorchList.pool.pData[elemIdx];
        scrSCORCH_MARK_DYN *mark = &node->val;
        scrSCORCH_DESC *desc = mark->desc.pointee;

        // Source object the dynamic batch follows (typed; was raw pBatch+0x188).
        objOBJ *src = mark->pBatch->desc.pObjFollow;
        if (src && (src->stateProc & 0x41100DF) == 0)
        {
            if (overlappableOnly && !desc->canBeOverlapped)
            {
                src->pInst->Validate(1); // member-call, 1 arg
                m3dV worldPos;
                src->matrLT.TransformPoint(&mark->pos, &worldPos);

                float radiusEff = desc->visibleAreaPart * mark->radius;
                float dist2 = m3dDist_2(&worldPos, &pos);
                if (dist2 < (radiusEff + radius) * (radiusEff + radius))
                {
                    if ((dist2 > (radiusEff - radius) * (radiusEff - radius) && radius < radiusEff * 2.0f)
                        || radius <= radiusEff)
                    {
                        osLOCK_Unlock(&g_syncMT, nullptr, 0);
                        return true;
                    }
                    if (dynToRemove.nEntry < 32)
                    {
                        ds::VAL_LIST<scrSCORCH_MARK_DYN>::ITERATOR it;
                        it.elemIdx = elemIdx;
                        it.pool = &this->dynScorchList.pool;
                        dynToRemove.Add(it);
                    }
                }
            }
            else if (classID.id == mark->classID.id)
            {
                src->pInst->Validate(1); // member-call, 1 arg
                m3dV worldPos;
                src->matrLT.TransformPoint(&mark->pos, &worldPos);

                float radiusEff = desc->radNoAround;
                if (radiusEff <= 0.0f)
                    radiusEff = desc->visibleAreaPart * mark->radius;
                float dist2 = m3dDist_2(&worldPos, &pos);
                if (dist2 < (radiusEff + radNoAround) * (radiusEff + radNoAround))
                {
                    if (desc->radNoAround > 0.0f)
                        radiusEff = desc->visibleAreaPart * mark->radius;
                    if ((dist2 > (radius - radiusEff) * (radius - radiusEff) && radius < radiusEff * 2.0f)
                        || radius <= radiusEff)
                    {
                        osLOCK_Unlock(&g_syncMT, nullptr, 0);
                        return true;
                    }
                    if (dynToRemove.nEntry < 32)
                    {
                        ds::VAL_LIST<scrSCORCH_MARK_DYN>::ITERATOR it;
                        it.elemIdx = elemIdx;
                        it.pool = &this->dynScorchList.pool;
                        dynToRemove.Add(it);
                    }
                }
            }
        }

        elemIdx = this->dynScorchList.pool.pData[elemIdx].nextIdx;
    }

    // ---- apply removals (reverse order, dynamic then static) ------------------------------------
    for (int i = dynToRemove.nEntry - 1; i >= 0; --i)
        RemoveScorchmark(dynToRemove.list[i]);

    for (int i = statToRemove.nEntry - 1; i >= 0; --i)
        RemoveScorchmark(*statToRemove.list[i].val, statToRemove.list[i].key);

    osLOCK_Unlock(&g_syncMT, nullptr, 0);
    return false;
}
