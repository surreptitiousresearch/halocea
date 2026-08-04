#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/nav/navWP.h"           // navWP::GetEffectivePos / GetRefObjectVector / GetDirToHOPlane
#include "../../headers/ws/ai/aiDEBUG_boundaries.h" // wpWP::TransformPoint
#include "../../headers/ws/m3d/m3d_boundary.h"      // m3dNormalize, m3dVZero, operator*/+/-

// aiHO_SYS::GetHOWantPos @ 0x832A5910
// ?GetHOWantPos@aiHO_SYS@@UBA?AUm3dV@@ABVnavWP@@W4HO_SHIFT@aiHO_SYS_BASE@@@Z
//
// Compute the desired HO world position for waypoint `howp` under `shift`, returned by value.
// (The decompiler swapped the hidden return-buffer pointer with `this`; disasm confirms r31 is the
// returned m3dV and r28 is the const aiHO_SYS — corrected below.)
m3dV aiHO_SYS::GetHOWantPos(const navWP &howp, HO_SHIFT shift) const
{
    m3dV want;
    howp.GetEffectivePos(&want);                 // r31 <- effective world position
    m3dV refObject = howp.GetRefObjectVector();  // v23 (corner/reference vector)
    m3dV dirToPlane = *howp.GetDirToHOPlane();   // v24

    switch (shift) {
    case HO_SHIFT_TO_CORNER: // 1 — shift straight to the reference vector
        want += refObject;
        break;

    case HO_SHIFT_DISABLED: { // 0 — add the projection of refObject onto the (normalized) plane dir
        m3dNormalize(&dirToPlane);
        float proj = dirToPlane.x * refObject.x + dirToPlane.y * refObject.y + dirToPlane.z * refObject.z;
        want += dirToPlane * proj;
        break;
    }

    case HO_SHIFT_TO_BODYPOS:   // 2
    case HO_SHIFT_TO_ENTERPOS: { // 3
        // Restart from the waypoint payload's local origin transformed into world space.
        howp.wpData->TransformPoint(&m3dVZero, &want);
        m3dNormalize(&dirToPlane);
        float proj = dirToPlane.x * refObject.x + dirToPlane.y * refObject.y + dirToPlane.z * refObject.z;
        m3dV parallel = dirToPlane * proj;    // v25 — component along the plane dir
        m3dV perp = refObject - parallel;     // v26 — residual (toward the corner)

        float coefAlong = (shift == HO_SHIFT_TO_BODYPOS) ? enterHOPenaltyCenterShiftToHO
                                                         : enterHOPosShiftToHO;
        float coefPerp  = (shift == HO_SHIFT_TO_BODYPOS) ? enterHOPenaltyCenterShiftToCorner
                                                         : enterHOPosShiftToCorner;
        want += parallel * coefAlong + perp * coefPerp;

        if (shift == HO_SHIFT_TO_BODYPOS) {
            // Record this body-entry want-pos on the owning brain (pBrain+0x1448 = "valid" flag,
            // pBrain+0x144C = the m3dV). aiBRAIN's internals at this offset are the next frontier, so
            // this is written through raw offsets rather than a fabricated field.
            char *brainRec = reinterpret_cast<char *>(pBrain);
            *reinterpret_cast<bool *>(brainRec + 0x1448) = true;
            *reinterpret_cast<m3dV *>(brainRec + 0x144C) = want;
        }
        break;
    }

    default: // shift > 3: only the effective position (no shift applied)
        break;
    }
    return want;
}
