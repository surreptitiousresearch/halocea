#include "../../headers/ws/msg/msgSYSTEM.h"

// 0x825077B8 — frame-skip guard for render preparation. Returns true (skip) when this frame's
// number has already been recorded, otherwise records it and returns false. `lastFrameNmb` is a
// translation-unit-local persistent value (a function-static in the original).
static unsigned int lastFrameNmb;

bool gsRendPrepareRenderNextFrameSkip()
{
    if (lastFrameNmb == msgSystem.frameNmb)
        return true;

    lastFrameNmb = msgSystem.frameNmb;
    return false;
}
