#include "m3dV.h"
#include "m3dOBB.h"
#include "m3dBOX.h"

// m3dOBB::CalcEnclBox @ 0x82A70FA8
// Compute the axis-aligned box that encloses this oriented box: gather the 8 corners,
// then fit an AABB to them (no extra expansion).
void m3dOBB::CalcEnclBox(m3dBOX *bb) const
{
    m3dV vertices[8];
    GetVertices(vertices);
    bb->Calc(vertices, 8, 0.0f);
}
