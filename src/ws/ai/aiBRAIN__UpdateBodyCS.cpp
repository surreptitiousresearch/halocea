#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/animINST.h"
#include "../../headers/ws/m3d/m3dMATR.h"

// m3d subsystem — zero vector constant (?m3dVZero@@3Um3dV@@A). Declared locally to avoid pulling
// in the ai boundary header (which re-defines aiSTATUS). boundary data.
extern const m3dV m3dVZero;

// aiBRAIN::UpdateBodyCS @ 0x8317A908
// ?UpdateBodyCS@aiBRAIN@@QAAXXZ
//
// Refresh the brain status' coordinate-system block: pull the body CS (pos/dir/up) and the face
// params, snap in the animation instance's local-to-world axes, and finally the upper-body
// direction from the body interface.
void aiBRAIN::UpdateBodyCS()
{
    m3dV pos, dir, dirUp;
    GetBodyCS(pos, dir, dirUp);
    aiSTATUS *status = this->st.pointee;
    status->cs.Set(pos, dir, dirUp);

    // GetFaceParam fills (dirFace, posFace) — matches the disasm's arg order.
    m3dV faceDir, facePos;
    GetFaceParam(&faceDir, &facePos);
    status = this->st.pointee;
    status->cs.posFace = facePos;
    status->cs.dirFace = faceDir;

    // The instance L2W rows map to the inst coordinate system: v[3]=translation (pos),
    // v[2]=+Z (forward), v[1]=+Y (up). m4dV rows are reinterpreted as m3dV (leading xyz).
    m3dMATR matL2W;
    this->pInst->GetMatrL2W(&matL2W);
    this->st.pointee->cs.Inst(*reinterpret_cast<const m3dV *>(&matL2W.v[3]),
                              *reinterpret_cast<const m3dV *>(&matL2W.v[2]),
                              *reinterpret_cast<const m3dV *>(&matL2W.v[1]));

    aiBODY_IFACE *pBody = this->body.pointee;
    m3dV upperDir = m3dVZero;
    pBody->GetUpperBodyDir(&upperDir);
    this->st.pointee->cs.UpperBody(upperDir);
}
