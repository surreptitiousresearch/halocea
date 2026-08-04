#include "../../headers/ws/ai/aiWATCHER_ENT.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dVUnitZ constant

// aiWATCHER_ENT::GetReticle(m3dV&) const @ 0x8328EFF0  (virtual)
// ?GetReticle@aiWATCHER_ENT@@UBA?AUm3dV@@AAU2@@Z
// Return the tracked entity's aim direction (instance forward axis) and write its aim origin
// (instance position) into `eye`. When the watcher is invalid, return the world +Z axis; `eye` is
// left untouched in that path (faithful to the binary — the else branch never writes it).
// NOTE (ABI): the decompiler swaps the sret return-value pointer with `this`; disassembly confirms
// r3 = return m3dV*, r4 = this, with entity read at this+0x4C.
m3dV aiWATCHER_ENT::GetReticle(m3dV &eye) const
{
    aiWATCHER_ENT *self = const_cast<aiWATCHER_ENT *>(this);
    if (self->IsValid())
    {
        entENTITY *ent = entity.pHandle ? entity.pHandle->pPtr : nullptr;
        m3dV dir;
        ent->pInst->GetDir(&dir);
        ent->pInst->GetPos(&eye);
        return dir;
    }
    return m3dVUnitZ;
}
