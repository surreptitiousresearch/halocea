#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_UpdateHeights_boundary.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/animINST.h"
#include "../../headers/ws/cdt/cdtSKEL.h"
#include "../../headers/ws/cdt/cdtBONE.h"
#include "../../headers/ws/m3d/m3dMATR.h"

// aiWATCHER::UpdateHeights @ 0x8328E130
// ?UpdateHeights@aiWATCHER@@IAAXXZ
// Recompute the watcher's cached body height and min-flesh height from the tracked entity's
// collision skeleton (head + stomach bones), when a refresh is pending and the watcher is valid.
// Clears the pending flag once the entity/skeleton/bones are unavailable.
void aiWATCHER::UpdateHeights()
{
    if (!mayUpdateHeights || !IsValid())
        return;

    FillHeightsWithProp();

    entENTITY *ent = GetEntity();
    if (!ent)
    {
        mayUpdateHeights = false;
        return;
    }

    animINST *inst = ent->pInst;
    cdtSKEL *skel = inst->GetCDTSkeleton();
    if (!skel)
    {
        mayUpdateHeights = false;
        return;
    }

    // Binary builds the local-to-world matrix here (unused by the bone-centre queries below).
    m3dMATR matrL2W = m3dMatrIdentity;
    inst->GetMatrL2W(&matrL2W);

    m3dV bodyUp;
    GetBodyDirUp(&bodyUp);
    m3dV bodyPos;
    GetBodyPos(&bodyPos);

    const cdtBONE *headBone = skel->FindBoneByName(PART_HEAD);
    const cdtBONE *stomachBone = skel->FindBoneByName(PART_STOMACH);
    if (headBone && stomachBone)
    {
        height = CalcYHeightFromMatrToBone(inst, bodyPos, bodyUp, headBone);
        float headHeight = height;
        float stomachHeight = CalcYHeightFromMatrToBone(inst, bodyPos, bodyUp, stomachBone);
        minHeightFlesh = stomachHeight;
        if (headHeight - stomachHeight < 0.2f)
            height = headHeight + 0.2f;
    }
    else
    {
        mayUpdateHeights = false;
    }
}
