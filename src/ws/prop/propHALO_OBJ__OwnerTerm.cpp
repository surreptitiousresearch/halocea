#include "../../headers/ws/prop/propHALO_OBJ.h"
#include "../../headers/ws/prop/propHALO_OBJ_DESC.h"
#include "../../headers/ws/gs/gsLVL_TIMER.h"
#include "../../headers/ws/msg/msgSYSTEM.h"

// 0x82AF0xxx (m3d) — shared epsilon constant (~0.000001f) used across the engine for
// near-zero float comparisons. boundary — not reversed here (single rodata float).
extern const float M3D_EPSILON_5;

// 0x823EB418
// CAVEAT: the decompiler resolved this->spDesc.pointee's field access as
// `pointee[1].sslScripts.__cl.file` (an out-of-bounds propBASE_DESC[1] index landing inside a
// dsVECTOR's apCL cookie). That is a type-propagation artifact: spDesc is statically typed
// dsSMART_PTR<propBASE_DESC,...> but at runtime, for a propHALO_OBJ, it points at a
// propHALO_OBJ_DESC (52-byte propBASE_DESC base + 44 bytes of derived fields). Disassembly shows
// a single `lfs f12, 0x5C(r10)` read — offset 0x5C (92) is exactly propHALO_OBJ_DESC::timeTerm.
// Reconstructed below as a downcast to propHALO_OBJ_DESC* and a direct field read, matching the
// disassembly exactly (confirmed against propHALO_OBJ_DESC's DB-verified layout).
void propHALO_OBJ::OwnerTerm()
{
    propHALO_OBJ_DESC *desc = static_cast<propHALO_OBJ_DESC *>(this->spDesc.pointee);

    // Record the moment this property was terminated.
    this->timeTerm = gsLvlTimer->timeLife;

    // If the descriptor's configured termination time is (approximately) zero, terminate the
    // owning actor immediately by posting a terminate message (2) rather than waiting.
    if (desc->timeTerm < M3D_EPSILON_5)
        msgSystem.PostMsg(this->pOwner, 2, nullptr);
}
