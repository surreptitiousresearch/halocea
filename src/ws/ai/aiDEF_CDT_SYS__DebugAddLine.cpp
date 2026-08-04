// aiDEF_CDT_SYS::DebugAddLine @ 0x832466F0
// ?DebugAddLine@aiDEF_CDT_SYS@@QAAXABUm3dV@@0@Z
//
// Append a debug ray (from -> to) to the fixed-capacity dbgRays array, but only while it has room
// (capacity 20). The trailing Back() call in the decompiler is the return value of Add() being
// referenced; it has no side effect and is dropped here.
#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/m3d/m3dV.h"

void aiDEF_CDT_SYS::DebugAddLine(const m3dV &from, const m3dV &to)
{
    if (dbgRays.nEntry < 20) {
        FROM_TO ray;
        ray.from = from;
        ray.to   = to;
        dbgRays.Add(ray);
    }
}
