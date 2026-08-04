#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"                    // st.pointee->body flags
#include "../../headers/ws/ai/aiBRAIN_DESC.h"                // owned species descriptor (abil + tower/smt fields)
#include "../../headers/ws/ai/aiWATCHER_PLAYER_fsm_boundary.h" // propFSM + eventHandler (fsmCB_HANDLER_dsSTRID) + ActivateCB
#include "../../headers/ws/fsm/fsmCB_HANDLER.h"              // templated RegisterCB<aiBRAIN,fsmPARAM_CDT>
#include "../../headers/ws/fsm/fsmMOVE_EVENT.h"              // cdt_processed_()
#include "../../headers/ws/fsm/fsmBODY_FSM_EVENT.h"          // disable_hits_()
#include "../../headers/ws/fsm/fsmPARAM_CDT.h"               // RegisterCB PARAM type
#include "../../headers/ws/mdl/mdlINFO_BASE.h"               // Get/SetCdtAbilities
#include "../../headers/ws/ap/apSTATE_T.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiBODY::ProcessInitLevel() @ 0x832AD5C8
// ?ProcessInitLevel@aiBODY@@UAAXXZ  (virtual)
//
// Per-level body init:
//  1. Subscribe the brain's OnCdtProcessed handler to the body FSM's "cdt_processed" event.
//  2. Derive the body's collision ability bits from the species descriptor's floor-place ability
//     and the current body state, then install them on the body model info.
//  3. For tower-man / smart-death units, disable hit reactions on the body FSM.
//
// The descriptor's dynamic type is aiBRAIN_DESC (its static type is dscDESC); the abilities and the
// tower/smt-death fields are read from the DB-verified aiBRAIN_DESC layout. The eventHandler member
// is the fsmCB_HANDLER_dsSTRID boundary view of the same fsmCB_HANDLER<dsSTRID> memory, so it is
// reinterpret_cast to the templated view to reach RegisterCB (same convention as
// aiWEAPON::SubscribeForTargetAndSpread).
void aiBODY::ProcessInitLevel()
{
    propFSM *fsm = bodyFsm;
    aiBRAIN *brain = pBrain;

    reinterpret_cast<fsmCB_HANDLER<dsSTRID> *>(&fsm->eventHandler)
        ->RegisterCB<aiBRAIN, fsmPARAM_CDT>(
            fsmMOVE_EVENT::cdt_processed_(), brain, &aiBRAIN::OnCdtProcessed);

    apSTATE_T<unsigned char> abilities = infoBodyMdl->GetCdtAbilities();
    const aiBRAIN_DESC *desc = reinterpret_cast<const aiBRAIN_DESC *>(brain->spDesc.pointee);
    bool floorPlace = desc->abil.floorPlace; // aiBRAIN_DESC::abil @0x1E8 + 0x05

    if (floorPlace) abilities.state |= 3;    else abilities.state &= 0xFC;
    if (floorPlace) abilities.state &= 0xF7; else abilities.state |= 8;
    if (!floorPlace || ((brain->st.pointee->body.val >> 20) & 1))
        abilities.state &= 0xFB;
    else
        abilities.state |= 4;

    infoBodyMdl->SetCdtAbilities(abilities);

    const aiBRAIN_DESC *d2 = reinterpret_cast<const aiBRAIN_DESC *>(brain->spDesc.pointee);
    bool smtDeath = d2->isSmtDeathEnabled && d2->smtDeathMaxDist >= 0.0f;
    if (d2->towerManStrength > 0.000001f || smtDeath)
        fsm->eventHandler.ActivateCB(fsmBODY_FSM_EVENT::disable_hits_(), nullptr);
}
