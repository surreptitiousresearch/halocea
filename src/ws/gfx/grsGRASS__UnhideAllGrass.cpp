// ?UnhideAllGrass@grsGRASS@@QAAXXZ @ 0x82723830
// Queue a "hide" request flagged unhideAll=1 onto grsGRASS::hideRequest; the grass update later
// consumes it and reveals every previously-hidden blade.
#include "../../headers/ws/gfx/grsGRASS.h"
#include "../../headers/ws/gfx/grsBLADE_HIDE_REQ.h"
#include "../../headers/ws/m3d/m3dV.h"

extern const m3dV m3dVZero; // boundary — zero-vector constant (m3d subsystem global)

void grsGRASS::UnhideAllGrass()
{
    // The decompiler seeds the temporary from an uninitialised stack slot; the fields it cares
    // about are all overwritten below via the returned element, so the initial contents are dead.
    grsBLADE_HIDE_REQ req;
    req.searchInUnits = false;

    grsBLADE_HIDE_REQ *pReq = &this->hideRequest.PushBack(req); // PushBack returns T& per DB (Back())
    pReq->unhideAll     = true;
    pReq->pos.x         = m3dVZero.x;
    pReq->pos.y         = m3dVZero.y;
    pReq->radius        = 0.0f;
    pReq->pos.z         = m3dVZero.z;
    pReq->searchInUnits = false;
}
