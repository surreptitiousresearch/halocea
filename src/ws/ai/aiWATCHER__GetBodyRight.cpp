// aiWATCHER::GetBodyRight @ 0x8328D858
// ?GetBodyRight@aiWATCHER@@UBA?AUm3dV@@XZ
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"   // _m3dCheckValid(const m3dV*)

extern const m3dV m3dVZero;                       // ?m3dVZero@@3Um3dV@@A

// Virtual const getter: the watched body's world-space right vector, computed as the cross product
// of the body forward (dir) and up columns of the body coordinate system (GetBodyCS).
m3dV *aiWATCHER::GetBodyRight(m3dV *result)
{
    m3dV pos;
    m3dV dir;
    m3dV up;
    GetBodyCS(&pos, &dir, &up);
    *result = m3dVZero;                 // defensive init (overwritten below), preserved from binary
    _m3dCheckValid(&dir);
    _m3dCheckValid(&up);
    // result = dir x up
    result->x = dir.y * up.z - dir.z * up.y;
    result->y = dir.z * up.x - dir.x * up.z;
    result->z = dir.x * up.y - dir.y * up.x;
    return result;
}
