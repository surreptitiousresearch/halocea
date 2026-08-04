#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiBRAIN::Transport(const m3dV&) @ 0x832AE950
// ?Transport@aiBRAIN@@UAAXABUm3dV@@@Z
//
// Instantly move the brain to world position `pos`: teleport the body, refresh the cached body
// coordinate system, and notify the best-behaviour module that the unit was teleported.
void aiBRAIN::Transport(const m3dV *pos)
{
    body.pointee->Transport(pos);
    UpdateBodyCS();
    mind.best.pointee->NoticeTransported();
}
