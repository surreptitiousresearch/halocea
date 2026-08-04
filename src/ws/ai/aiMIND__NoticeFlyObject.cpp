#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPERC_BASE.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

extern const m3dV m3dVZero; // ?m3dVZero@@3Um3dV@@A

// aiMIND::NoticeFlyObject @ 0x83228158
// ?NoticeFlyObject@aiMIND@@QAAXABUm3dV@@0@Z
//
// A physics object flew past starting at `posFrom` travelling along `dirFly`. Extend the flight to
// a short segment, find the closest approach to the brain's body, record the throw geometry for the
// debug view, and notify the perception + interop/social (isa) modules.
void aiMIND::NoticeFlyObject(const m3dV &posFrom, const m3dV &dirFly)
{
    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    _m3dCheckValid(&posFrom);
    _m3dCheckValid(&dirFly);
    _m3dCheckValid(2.0f);

    // Segment end: two units of flight ahead of the start.
    m3dV posEnd;
    posEnd.x = dirFly.x * 2.0f + posFrom.x;
    posEnd.y = dirFly.y * 2.0f + posFrom.y;
    posEnd.z = dirFly.z * 2.0f + posFrom.z;

    m3dV posBody = brain->GetPos();

    m3dV posClosest = m3dVZero;
    m3dDistPointLine(&posBody, &posFrom, &posEnd, &posClosest);

    brain->dbg.posThrowObjStart   = posFrom;
    brain->dbg.dirThrowObj        = dirFly;
    brain->dbg.posThrowObjClosest = posClosest;

    // NOTE: the decompiler modelled the perception call as taking (m3dV*, float); the vtable slot's
    // mangled type ?NoticeFlyObject@aiPERC_BASE@@UAAXABUm3dV@@@Z is a single const m3dV&. The
    // distance value was a leftover fp register, not an argument. Corrected to one argument.
    perc.pointee->NoticeFlyObject(posClosest);
    isa.pointee->NotifyPhysObject(posClosest);
}
