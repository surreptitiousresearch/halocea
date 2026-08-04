#include "../../headers/ws/ai/aiFEE.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dDist

// boundary: virtual at aiBRAIN vtable +0x1C0 — returns the viewer's reference (eye) position by
// value (sret). Concrete slot/name unresolved this batch (candidate: aiBRAIN::GetPos).
static m3dV ViewerRefPos(const aiBRAIN *viewer)
{
    typedef m3dV *(*GetRefPosFn)(m3dV *ret, const aiBRAIN *self);
    const char *vtbl = *reinterpret_cast<const char *const *>(viewer);
    GetRefPosFn getRefPos = *reinterpret_cast<const GetRefPosFn *>(vtbl + 0x1C0);
    m3dV out;
    getRefPos(&out, viewer);
    return out;
}

// boundary: pointer at aiBRAIN +0x70 to the descriptor's distance-weight float array.
static const float *ViewerDistCoeffs(const aiBRAIN *viewer)
{
    return *reinterpret_cast<const float *const *>(reinterpret_cast<const char *>(viewer) + 0x70);
}

// aiFEE::CalculateDistanceForRanged(const aiBRAIN*) @ 0x8321FE20
// ?CalculateDistanceForRanged@aiFEE@@QAAXPBVaiBRAIN@@@Z
void aiFEE::CalculateDistanceForRanged(const aiBRAIN *viewer)
{
    aiWTC_HOLDER *holder = this->hold;
    aiWATCHER *watcher = holder->watcher;

    m3dV bodyPos;
    watcher->GetBodyPos(&bodyPos);

    m3dV viewerPos = ViewerRefPos(viewer);
    this->dist = m3dDist(&viewerPos, &bodyPos);

    this->isInOpen = !watcher->IsHideoutMode() || watcher->IsMiniZoomMode();
    this->isPlayer = watcher->IsPlayer();

    bool inOpen = this->isInOpen;
    bool isPlayer = this->isPlayer;
    float dist = this->dist;
    const float *coeff = ViewerDistCoeffs(viewer);

    if (inOpen)
    {
        if (isPlayer)
            this->dist4cmp = coeff[192] * dist;
        else
            this->dist4cmp = coeff[194] * dist;
    }
    else if (isPlayer)
    {
        this->dist4cmp = coeff[193] * dist;
    }
    else
    {
        this->dist4cmp = coeff[195] * dist;
    }
}
