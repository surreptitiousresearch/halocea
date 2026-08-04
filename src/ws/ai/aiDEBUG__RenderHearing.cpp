// ws-engine ai08 (a8_debug.cpp): aiDEBUG::RenderHearing @0x83199FC8 (protected).
// Draw the brain's three hearing-radius circles (walk / run / gunshot) centred on the body, in the
// plane spanned by the body forward/up directions, each tinted a distinct colour.
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_render_boundaries.h" // CreateHearingCircle, RenderPolyline, dlFree
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiSENSES.h"                  // radHearWalk/radHearRun/radHearShot
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/apCL.h"

static const char *const kSrcFile =
    "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_debug.cpp";

void aiDEBUG::RenderHearing()
{
    aiSTATUS *st = this->pBrain->st.pointee;

    // NOTE: deviation — the decompiler overlaid the pos/dir/normal input m3dVs and the two working
    // vectors onto overlapping stack slots (writing body-CS floats into the vectors' pData/nElem/
    // __cl words). Collapsed here to the three named source m3dVs plus the render/work vectors.
    m3dV pos    = st->cs.posBody;
    m3dV dir    = st->cs.dirBody;
    m3dV normal = st->cs.dirUpBody;

    apCL cl = { kSrcFile, 1686 };
    dsVECTOR<m3dV, 8> circle(cl);   // rendered polyline
    dsVECTOR<m3dV, 8> work(cl);     // CreateHearingCircle working set

    CreateHearingCircle(&pos, st->perc.senses.radHearWalk, &dir, &normal, &circle, &work);
    RenderPolyline(&circle, 0xFFFF0000);

    CreateHearingCircle(&pos, st->perc.senses.radHearRun, &dir, &normal, &circle, &work);
    RenderPolyline(&circle, 0x0FFFFF00);

    CreateHearingCircle(&pos, st->perc.senses.radHearShot, &dir, &normal, &circle, &work);
    RenderPolyline(&circle, 0xFF00FF00);

    dlFree(circle.pData);
}
