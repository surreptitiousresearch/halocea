#include "../../headers/ws/rend/shCOLOR.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3dCOLOR.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// shCOLOR::AddLight @ 0x83096240  (?AddLight@shCOLOR@@QAAXPAUm3dV@@0MPAVm3dCOLOR@@@Z)
//
// Positional overload: derive the unit direction from receiver `pos` to `lightPos`, scale the
// light colour by `intensity`, then forward to the directional AddLight.
//
// ABI: the mangling has four params — (m3dV* pos, m3dV* lightPos, float intensity,
// m3dCOLOR* color). The decompiler's extra `a6` float* is the same colour pointer it actually
// reads (r,g,b,a); the phantom `color` param is dropped.

void shCOLOR::AddLight(const m3dV *pos, const m3dV *lightPos, float intensity,
                       const m3dCOLOR *color)
{
    m3dV dir;
    dir.x = lightPos->x - pos->x;
    dir.y = lightPos->y - pos->y;
    dir.z = lightPos->z - pos->z;
    m3dNormalize(&dir);

    m3dCOLOR scaled;
    scaled.r = color->r * intensity;
    scaled.g = color->g * intensity;
    scaled.b = color->b * intensity;
    scaled.a = color->a * intensity;

    AddLight(&dir, &scaled);
}
