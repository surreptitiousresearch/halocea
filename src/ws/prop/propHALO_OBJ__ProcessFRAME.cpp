#include "../../headers/ws/prop/propHALO_OBJ.h"
#include "../../headers/ws/prop/propHALO_OBJ_DESC.h"
#include "../../headers/ws/gs/gsLVL_TIMER.h"
#include "../../headers/ws/msg/msgSYSTEM.h"

// propHALO_OBJ::ProcessFRAME() @ 0x823EB460
// Once OwnerTerm has stamped timeTerm, and the descriptor's configured grace period has elapsed
// since then, post a terminate message (2) to the owning actor.
//
// CAVEAT: same spDesc-downcast type-propagation artifact documented in OwnerTerm.cpp -- the
// decompiler resolved the descriptor field read as `spDesc.pointee[1].sslScripts.__cl.file`, but
// the disassembly (`lfs f13, 0x5C(r10)` where r10 = spDesc.pointee) is the same single-float read
// of propHALO_OBJ_DESC::timeTerm at offset 0x5C, reconstructed here as a downcast + direct field
// read exactly as in OwnerTerm.
void propHALO_OBJ::ProcessFRAME()
{
    if (this->timeTerm <= 0.0f)
        return;

    propHALO_OBJ_DESC *desc = static_cast<propHALO_OBJ_DESC *>(this->spDesc.pointee);
    if (gsLvlTimer->timeLife - this->timeTerm > desc->timeTerm)
        msgSystem.PostMsg(this->pOwner, 2, nullptr);
}
