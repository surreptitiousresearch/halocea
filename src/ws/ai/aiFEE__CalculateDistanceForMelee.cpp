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

// aiFEE::CalculateDistanceForMelee(const aiBRAIN*) @ 0x8321FD38
// ?CalculateDistanceForMelee@aiFEE@@QAAXPBVaiBRAIN@@@Z
void aiFEE::CalculateDistanceForMelee(const aiBRAIN *viewer)
{
    aiWTC_HOLDER *holder = this->hold;
    aiWATCHER *watcher = holder->watcher;

    m3dV bodyPos;
    watcher->GetBodyPos(&bodyPos);

    m3dV viewerPos = ViewerRefPos(viewer);
    this->dist = m3dDist(&viewerPos, &bodyPos);

    const float *coeff = ViewerDistCoeffs(viewer);
    float minMeleeDist = coeff[189];

    this->isPlayer = watcher->IsPlayer();
    if (this->isPlayer)
    {
        if (this->dist >= minMeleeDist)
            this->dist4cmp = coeff[196] * this->dist;
        else
            this->dist4cmp = 0.0f;
    }
    else
    {
        this->dist4cmp = coeff[197] * this->dist;
    }
}
