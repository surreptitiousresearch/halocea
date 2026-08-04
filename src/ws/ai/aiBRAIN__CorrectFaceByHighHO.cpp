#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"        // st.pointee->peekerSelf / cs
#include "../../headers/ws/ai/aiWATCHER.h"        // IsInHighHO / IsMiniZoomMode / IsHOSideLeft/Right
#include "../../headers/ws/ai/aiBRAIN_DESC.h"     // shiftPosFace{Hor,Vert}InHighHO
#include "../../headers/ws/dsc/dscDESC.h"          // spDesc.pointee element type
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"     // operator* / operator- / operator+

// aiBRAIN::CorrectFaceByHighHO(m3dV*, m3dV*) @ 0x832AE088
// ?CorrectFaceByHighHO@aiBRAIN@@UBAXPAUm3dV@@0@Z
//
// When this unit's self-peeker is in a high hideout (and not in mini-zoom), shift the face position
// `pos` sideways/upward by the descriptor's horizontal/vertical high-hideout face offsets. The side
// (left/right) selects the sign of the horizontal shift. `dir` is unused. Does nothing when `pos`
// is null. (The peeker is the aiWATCHER base of the aiWATCHER_BRAIN — reinterpreted to reach the
// aiWATCHER hideout query surface, matching the ABI single-inheritance layout.)
void aiBRAIN::CorrectFaceByHighHO(m3dV * /*dir*/, m3dV *pos) const
{
    if (!pos)
        return;

    aiSTATUS *status = st.pointee;
    aiWATCHER *watcher = reinterpret_cast<aiWATCHER *>(status->peekerSelf);
    if (!watcher)
        return;
    if (!watcher->IsInHighHO() || watcher->IsMiniZoomMode())
        return;

    // spDesc.pointee is the owned dscDESC, which is really this brain's aiBRAIN_DESC.
    aiBRAIN_DESC *desc = reinterpret_cast<aiBRAIN_DESC *>(spDesc.pointee);

    if (watcher->IsHOSideLeft())
    {
        aiSTATUS::CS &cs = st.pointee->cs;
        m3dV up = desc->shiftPosFaceVertInHighHO * cs.dirUpBody;
        m3dV right = desc->shiftPosFaceHorInHighHO * cs.dirRightBody;
        *pos = (cs.posBody - right) + up;
    }
    else if (watcher->IsHOSideRight())
    {
        aiSTATUS::CS &cs = st.pointee->cs;
        m3dV up = desc->shiftPosFaceVertInHighHO * cs.dirUpBody;
        m3dV right = desc->shiftPosFaceHorInHighHO * cs.dirRightBody;
        *pos = (cs.posBody + right) + up;
    }
}
