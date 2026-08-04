// aiBRAIN::implGetDistPlayerToPathEnd @ 0x832AF338
// ?implGetDistPlayerToPathEnd@aiBRAIN@@UAAMXZ
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPlanner
#include "../../headers/ws/m3d/m3dV.h"

// Distance from the first live tracked player to the end of the brain's active PATH behaviour
// (best-behaviour id 4), or -1 when there is no path / no player. The player's nav-space body
// position (aiWATCHER::GetBodyPosNav, vtable byte offset 0x28) is fed to the bhvPATH dist-to-path-end
// getter (vtable byte offset 0xFC); both are dispatched by raw slot to avoid fabricating vtables.
float aiBRAIN::implGetDistPlayerToPathEnd()
{
    void *pathBhv = mind.best.pointee->FindBHV_2(4);
    if (!pathBhv || !aiPlanner)
        return -1.0f;

    int nPlayers = aiPlanner->wah.arrPlayers.nElem;
    if (nPlayers <= 0)
        return -1.0f;

    aiWATCHER *player = nullptr;
    for (int i = 0; i < nPlayers; ++i)
    {
        player = aiPlanner->wah.PeekPlayer(i);
        if (player)
            break;
    }
    if (!player)
        return -1.0f;

    // aiWATCHER::GetBodyPosNav (vtable byte offset 0x28) — nav-space body position (sret).
    m3dV bodyPos;
    void **playerVtbl = *reinterpret_cast<void ***>(player);
    typedef void (*GetBodyPosNavFn)(void *, m3dV *);
    GetBodyPosNavFn getBodyPosNav =
        reinterpret_cast<GetBodyPosNavFn>(playerVtbl[0x28 / sizeof(void *)]);
    getBodyPosNav(player, &bodyPos);

    // bhvPATH dist-to-path-end (vtable byte offset 0xFC) — float distance from `bodyPos`.
    void **pathVtbl = *reinterpret_cast<void ***>(pathBhv);
    typedef float (*DistToEndFn)(void *, m3dV *);
    DistToEndFn distToEnd = reinterpret_cast<DistToEndFn>(pathVtbl[0xFC / sizeof(void *)]);
    return distToEnd(pathBhv, &bodyPos);
}
