#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // m3dUpdateTimeField

// aiWATCHER::Update @ 0x8328FDF8
// ?Update@aiWATCHER@@UAA_NM@Z
// Virtual per-frame tick. When the watcher is invalid or the target is dead, zero every timer and
// report inactive. Otherwise advance the hideout / open timers (a watcher counts as "in hideout"
// only when in hideout mode and not mini-zooming), decay the logical-shield timer, refresh the
// game-info props and cached body heights.
bool aiWATCHER::Update(float dt)
{
    if (!IsValid() || IsDead())
    {
        timeOpen = 0.0f;
        timeInHO = 0.0f;
        timeInHOMode = 0.0f;
        timerLogicalShield = 0.0f;
        return false;
    }

    if (IsHideoutMode() && !IsMiniZoomMode())
    {
        timeInHO += dt;
        timeOpen = 0.0f;
        hideoutDirLastUsed = GetHideoutDir(); // virtual (sret)
    }
    else
    {
        timeOpen += dt;
        timeInHO = 0.0f;
    }

    if (IsHideoutMode())
        timeInHOMode += dt;
    else
        timeInHOMode = 0.0f;

    m3dUpdateTimeField(&timerLogicalShield, dt);
    FillGameInfoProp();
    UpdateHeights();
    return true;
}
