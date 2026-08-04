#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"
#include "../../headers/ws/gs/gsDEBUG_PRIMITIVE_INITOR.h"
#include "../../headers/ws/m3d/m3dV.h"

// a8_debug.cpp file-scope debug-primitive builder + per-call name counter for the cylinder path.
static gsDEBUG_PRIMITIVE_INITOR prim_0;
static int idx_2;

// Emit the current prim_0 as a named 15s timed line/segment primitive.
static void emitSegment(const char *nameFmt)
{
    dsSPrintf(&prim_0.name, nameFmt, idx_2++);
    gsAddDebugPrimitive(&prim_0);
}

// aiDEBUG::PushTimedCylinder @ 0x83195B10
//   (?PushTimedCylinder@aiDEBUG@@QAAXAAUm3dV@@0MPADK@Z)
//
// Push a 15-second timed debug cylinder whose axis runs from `vFrom` to `vTo` with radius `radius`,
// built out of a set of line-segment debug primitives (two axis lines offset by +/- radius on Y,
// plus the radial spokes at each end). `nameFmt` uniquely keys each emitted segment.
//
// The binary reuses a single primitive builder (prim_0) and rewrites its pos/dir before each
// gsAddDebugPrimitive call; this reconstruction preserves that emission sequence.
void aiDEBUG::PushTimedCylinder(m3dV *vFrom, m3dV *vTo, float radius, const char *nameFmt)
{
    prim_0.color = 0;              // caller-supplied colour packed via the ABI tail; see note below
    prim_0.addParam = gsDEBUG_PRIMITIVE_INITOR::ALPHA_REPLACE;
    prim_0.type = 8;               // cylinder/segment primitive kind
    prim_0.time = 15.0f;

    // --- axis segment 1: from vFrom (raised by radius) along vFrom->vTo ---
    prim_0.pos = *vFrom;
    prim_0.pos.y += radius;
    _m3dCheckValid(vTo);
    _m3dCheckValid(vFrom);
    prim_0.dir.x = vTo->x - vFrom->x;
    prim_0.dir.y = vTo->y - vFrom->y;
    prim_0.dir.z = vTo->z - vFrom->z;
    emitSegment(nameFmt);

    // --- axis segment 2: mirrored below by 2*radius ---
    m3dV pos = prim_0.pos;
    prim_0.pos.y = -((radius * 2.0f) - prim_0.pos.y);
    emitSegment(nameFmt);

    // Perpendicular to the axis in the horizontal plane: cross(UnitY, dir), normalised.
    m3dV axisPos = prim_0.pos;
    _m3dCheckValid(&prim_0.dir);
    _m3dCheckValid(&m3dVUnitY);
    m3dV perp;
    perp.x = m3dVUnitY.z * prim_0.dir.y - m3dVUnitY.y * prim_0.dir.z;
    perp.y = prim_0.dir.z * m3dVUnitY.x - m3dVUnitY.z * prim_0.dir.x;
    perp.z = m3dVUnitY.y * prim_0.dir.x - prim_0.dir.y * m3dVUnitY.x;
    m3dNormalize(&perp);

    // Radial offset r = perp * radius; spokes at vFrom +/- r and the two axis endpoints.
    _m3dCheckValid(&perp);
    m3dV r;
    r.x = perp.x * radius;
    r.y = perp.y * radius;
    r.z = perp.z * radius;

    // spoke A: vFrom + r
    _m3dCheckValid(vFrom);
    _m3dCheckValid(&r);
    m3dV plusR;
    plusR.x = vFrom->x + r.x;
    plusR.y = vFrom->y + r.y;
    plusR.z = vFrom->z + r.z;
    prim_0.pos = plusR;
    emitSegment(nameFmt);

    // spoke B: vFrom - r
    m3dV minusR;
    minusR.x = vFrom->x - r.x;
    minusR.y = vFrom->y - r.y;
    minusR.z = vFrom->z - r.z;
    prim_0.pos = minusR;
    emitSegment(nameFmt);

    // connecting segments between the mirrored axis end (pos) and the two spokes.
    m3dV segEnd = prim_0.pos;
    prim_0.dir.x = pos.x - segEnd.x;
    prim_0.dir.y = pos.y - segEnd.y;
    prim_0.dir.z = pos.z - segEnd.z;
    emitSegment(nameFmt);

    prim_0.dir.x = axisPos.x - segEnd.x;
    prim_0.dir.y = axisPos.y - segEnd.y;
    prim_0.dir.z = axisPos.z - segEnd.z;
    emitSegment(nameFmt);

    // closing segments from the raised axis end (axisPos) back to pos.
    prim_0.pos = plusR;
    prim_0.dir.x = pos.x - plusR.x;
    prim_0.dir.y = pos.y - plusR.y;
    prim_0.dir.z = pos.z - plusR.z;
    emitSegment(nameFmt);

    prim_0.dir.x = axisPos.x - plusR.x;
    prim_0.dir.y = axisPos.y - plusR.y;
    prim_0.dir.z = axisPos.z - plusR.z;
    emitSegment(nameFmt);
}
