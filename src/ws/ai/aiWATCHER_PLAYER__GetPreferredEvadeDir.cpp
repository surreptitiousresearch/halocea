#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"
#include "../../headers/ws/m3d/m3d_boundary.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // canonical m3dVZero (unused fallback ref)

// aiWATCHER_PLAYER::GetPreferredEvadeDir @ 0x8328DD40
// ?GetPreferredEvadeDir@aiWATCHER_PLAYER@@UBA?AUm3dV@@ABU2@@Z
// Virtual const: the preferred lateral evade direction. Defaults to the negated cached body forward.
// If that forward is nearly parallel to the (up-flattened) offset from the player to `posEvader`
// — i.e. the cross product is small relative to the offset — fall back to bodyUp x bodyDir instead.
m3dV aiWATCHER_PLAYER::GetPreferredEvadeDir(const m3dV &posEvader) const
{
    _m3dCheckValid(&cachedBodyDir);

    m3dV result;
    result.x = -cachedBodyDir.x;
    result.y = -cachedBodyDir.y;
    result.z = -cachedBodyDir.z;

    _m3dCheckValid(&cachedBodyPos);
    _m3dCheckValid(&posEvader);

    // Offset from the evader toward the player body.
    m3dV rel;
    rel.x = cachedBodyPos.x - posEvader.x;
    rel.y = cachedBodyPos.y - posEvader.y;
    rel.z = cachedBodyPos.z - posEvader.z;

    // Remove the component along the body-up axis (flatten into the horizontal plane).
    float upProj = cachedBodyUp.x * rel.x + cachedBodyUp.y * rel.y + cachedBodyUp.z * rel.z;
    _m3dCheckValid(upProj);
    _m3dCheckValid(&cachedBodyUp);
    rel.x -= cachedBodyUp.x * upProj;
    rel.y -= cachedBodyUp.y * upProj;
    rel.z -= cachedBodyUp.z * upProj;

    _m3dCheckValid(&rel);
    _m3dCheckValid(&result);

    // Cross(result, rel): only its (squared) magnitude is used as a parallelism/degeneracy test.
    m3dV crossCheck;
    crossCheck.x = result.z * rel.y - result.y * rel.z;
    crossCheck.y = result.x * rel.z - result.z * rel.x;
    crossCheck.z = result.y * rel.x - result.x * rel.y;

    if (m3dLengthVector_2(&crossCheck) * 9.0f <= m3dLengthVector_2(&rel))
        m3dCrossProduct(&cachedBodyUp, &cachedBodyDir, &result); // out = bodyUp x bodyDir

    return result;
}
