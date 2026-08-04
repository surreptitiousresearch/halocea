#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiBRAIN_IFACE::GetBodyPosNav() const @ 0x83193558
// Nav-space body reference position = body position + 0.75 * body up-direction.
//
// The two geometry accessors are dispatched through fixed vtable slots on this object (offsets
// +456 and +448) whose named virtuals live outside this batch's descent depth. Reproduced
// faithfully by offset to preserve binary fidelity; both are const, sret-returning m3dV methods.
m3dV aiBRAIN_IFACE::GetBodyPosNav() const
{
    typedef void (*BodyGeomFn)(m3dV *out, const aiBRAIN_IFACE *self); // sret, thiscall
    const void *const *vtbl = *reinterpret_cast<const void *const *const *>(this);

    m3dV bodyUp;
    reinterpret_cast<BodyGeomFn>(vtbl[456 / 4])(&bodyUp, this);

    m3dV bodyPos;
    reinterpret_cast<BodyGeomFn>(vtbl[448 / 4])(&bodyPos, this);

    _m3dCheckValid(0.75f);
    _m3dCheckValid(&bodyUp);

    m3dV scaledUp;
    scaledUp.x = bodyUp.x * 0.75f;
    scaledUp.y = bodyUp.y * 0.75f;
    scaledUp.z = bodyUp.z * 0.75f;

    _m3dCheckValid(&bodyPos);
    _m3dCheckValid(&scaledUp);

    m3dV result;
    result.x = scaledUp.x + bodyPos.x;
    result.y = bodyPos.y + scaledUp.y;
    result.z = bodyPos.z + scaledUp.z;
    return result;
}
