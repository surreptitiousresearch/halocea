#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"
#include "../../headers/ws/gs/gsDEBUG_PRIMITIVE_INITOR.h"
#include "../../headers/ws/m3d/m3dV.h"

// a8_debug.cpp file-scope debug-primitive builder + per-call name counter (function-local statics
// in the binary; the first-use guard + atexit dtor register the same lifetime).
static gsDEBUG_PRIMITIVE_INITOR prim;
static int idx_1;

// aiDEBUG::PushTimedDot @ 0x831956E0  (?PushTimedDot@aiDEBUG@@QAAXAAUm3dV@@PADK@Z)
//
// Push a 15-second timed debug dot primitive at `pos`, named via `nameFmt` (formatted with a
// monotonically increasing index so each primitive is uniquely keyed).
void aiDEBUG::PushTimedDot(m3dV *pos, unsigned int color, const char *nameFmt)
{
    prim.color = color;
    prim.type = 3;                                       // dot primitive kind
    prim.addParam = gsDEBUG_PRIMITIVE_INITOR::ALPHA_REPLACE;
    prim.time = 15.0f;
    dsSPrintf(&prim.name, nameFmt, idx_1++);
    prim.pos = *pos;
    prim.size = 0.029999999f;
    gsAddDebugPrimitive(&prim);
}
