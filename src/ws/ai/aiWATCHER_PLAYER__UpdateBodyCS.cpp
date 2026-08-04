#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/cam/camCAMERA.h"
#include "../../headers/ws/m3d/m3d_boundary.h"          // m3dVZero/UnitY/UnitZ, _m3dCheckValid, _m3dNormalize
#include "../../headers/ws/ai/aiWATCHER_batch_boundaries.h" // operator*(float,m3dV)
#include "../../headers/ws/ai/aiBRAIN_boundary.h"        // aiGetBodyCS
#include "../../headers/ws/ai/aiWATCHER_props_boundary.h" // MDL_ST_PROP_WLN
#include "../../headers/ws/ds/dsBIT_ARRAY_IMPL.h"

// boundary: the `use` subsystem's usePROCESSOR layout is unresolved in the DB and beyond this
// batch's descent. UpdateBodyCS tests the body's WLN (weapon-line/wall-lean) model-state property
// via dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_WLN.id, useProcessor->context.propList.props.array, 12).
// The processor's cached model-state property bit-array is exposed here as a boundary accessor to
// be resolved by the next `use`-subsystem frontier.  UNRESOLVED: usePROCESSOR field offsets.
struct usePROCESSOR;
extern const unsigned int *usePROCESSOR_ModelStatePropArray(usePROCESSOR *proc);

// 0x8285AB98 (?gsGetPlayerCam@@YAPAVcamCAMERA@@PAVentENTITY@@@Z) — the active player camera for the
// entity's local viewport (null when none).  boundary.
extern camCAMERA *gsGetPlayerCam(entENTITY *ent);

// aiWATCHER_PLAYER::UpdateBodyCS @ 0x8328FA58
// ?UpdateBodyCS@aiWATCHER_PLAYER@@IAAXXZ
//
// Refresh the cached body coordinate system for the frame. When the watcher is valid: pull the
// body CS from the game-info/body FSM (respecting the WLN model-state flag), derive the face point
// (body position raised 1.5 along the up vector), and build the reticle ray from the player camera
// (falling back to the body forward when the camera z-axis degenerates). When invalid: reset to a
// canonical identity CS (origin / +Z forward / +Y up).
void aiWATCHER_PLAYER::UpdateBodyCS()
{
    if (this->IsValid()) {
        this->FillGameInfoProp();
        if (this->pGameInfo) {
            usePROCESSOR *useProcessor = this->useProcessor;
            if (useProcessor) {
                bool weaponLine = dsBIT_ARRAY_IMPL::Is(
                    MDL_ST_PROP_WLN.id, usePROCESSOR_ModelStatePropArray(useProcessor), 12);
                aiGetBodyCS(this->pGameInfo, this->bodyFsm, weaponLine,
                            this->cachedBodyPos, this->cachedBodyDir, this->cachedBodyUp);
            }
        }

        // Face point/dir: body position raised 1.5 units along the (validated) body up vector.
        m3dV raisedUp = 1.5f * this->cachedBodyUp;
        _m3dCheckValid(&this->cachedBodyPos);
        _m3dCheckValid(&raisedUp);
        this->cachedFaceDir = this->cachedBodyDir;
        this->cachedFacePos.x = raisedUp.x + this->cachedBodyPos.x;
        this->cachedFacePos.y = raisedUp.y + this->cachedBodyPos.y;
        this->cachedFacePos.z = raisedUp.z + this->cachedBodyPos.z;

        // Reticle ray from the player camera (camera-to-world translation and forward column).
        entENTITY *ent = this->GetEntity();
        camCAMERA *playerCam = gsGetPlayerCam(ent);
        if (playerCam) {
            this->cachedReticleStart.x = playerCam->matrC2W.elements[12];
            this->cachedReticleStart.y = playerCam->matrC2W.elements[13];
            this->cachedReticleStart.z = playerCam->matrC2W.elements[14];
            this->cachedReticleDir.x = playerCam->matrC2W.elements[8];
            this->cachedReticleDir.y = playerCam->matrC2W.elements[9];
            this->cachedReticleDir.z = playerCam->matrC2W.elements[10];
            if (!_m3dNormalize(&this->cachedReticleDir))
                this->cachedReticleDir = this->cachedBodyDir;
        }
    } else {
        this->cachedBodyPos = m3dVZero;
        this->cachedBodyDir = m3dVUnitZ;
        this->cachedBodyUp = m3dVUnitY;
        this->cachedReticleDir = m3dVUnitZ;
        this->cachedReticleStart = this->cachedBodyPos; // == m3dVZero
    }
}
