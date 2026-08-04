#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"                    // pBrain->st.pointee (cs / perc)
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // m3dVZero + _m3dCheckValid/_m3dNormalize
#include "../../headers/ws/m3d/m3dV.h"

// aiVISION_COMMON::CalculateSightDir() const @ 0x83291130
// ?CalculateSightDir@aiVISION_COMMON@@IBA?AUm3dV@@XZ
// Protected const, sret (PPC — the decompiler mislabels the hidden return pointer as `this`): the
// effective sight direction. Normally the body face direction; but when the face direction is
// tilted the "wrong" way relative to the body up-axis and the corresponding up/down clip flag is
// set, the sight is redirected along (bodyRight x bodyUp) (falling back to the body forward
// direction when that cross product is degenerate).
m3dV *aiVISION_COMMON::CalculateSightDir(m3dV *out) const
{
    aiSTATUS *st = pBrain->st.pointee;

    m3dV dirUp   = st->cs.dirUpBody;
    m3dV dirFace = st->cs.dirFace;
    *out = dirFace;

    float dot = dirFace.x * dirUp.x + dirFace.y * dirUp.y + dirFace.z * dirUp.z;
    bool clipUp   = dot > 0.0f && st->perc.isClipUpAngle;
    bool clipDown = dot < 0.0f && st->perc.isClipDownAngle;

    if (clipUp || clipDown)
    {
        m3dV dirRight = st->cs.dirRightBody;
        m3dV cross = m3dVZero;
        _m3dCheckValid(&dirUp);
        _m3dCheckValid(&dirRight);
        cross.x = dirRight.z * dirUp.y - dirRight.y * dirUp.z;
        cross.y = dirRight.x * dirUp.z - dirRight.z * dirUp.x;
        cross.z = dirRight.y * dirUp.x - dirRight.x * dirUp.y;
        if (_m3dNormalize(&cross))
            *out = cross;
        else
            *out = st->cs.dirBody;
    }

    return out;
}
