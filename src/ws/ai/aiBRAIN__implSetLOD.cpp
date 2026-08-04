#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ent/entENTITY.h"    // base entENTITY::implSetLOD
#include "../../headers/ws/ai/aiBODY_IFACE.h"  // body.pointee->wpn
#include "../../headers/ws/ai/aiWEAPON_BASE.h" // wpn.pointee->SetLOD

// aiBRAIN::implSetLOD(int) @ 0x832AE030
// ?implSetLOD@aiBRAIN@@UAAXH@Z
//
// Virtual: forward the level-of-detail index to the base entity, then to the weapon module.
void aiBRAIN::implSetLOD(int idx)
{
    entENTITY::implSetLOD(idx);
    body.pointee->wpn.pointee->SetLOD(idx);
}
