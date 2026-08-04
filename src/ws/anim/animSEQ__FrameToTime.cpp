#include "animSEQ.h"

// m3dClamp(value, loEdgeValue, loEdge, hiEdge, x) — remap x within [loEdge,hiEdge]; boundary.
float m3dClamp(float a, float b, float c, float d, float e); // boundary — m3d math

// animSEQ::FrameToTime @ 0x8252C3C8 (const)
// Convert a frame index into a clamped time in seconds. The mangle (M M) makes both the return
// and the parameter float; the decompiler widened them to double.
float animSEQ::FrameToTime(float frame) const
{
    int stateSeq = this->stateSeq;
    if ((stateSeq & 0x100) == 0)
        return 0.0f;
    if ((stateSeq & 0x200) != 0) // reverse-time sequence
        return m3dClamp(0.0f, this->timeSec, this->startFrame, this->endFrame, frame);
    return m3dClamp(this->timeSec, 0.0f, this->endFrame, this->startFrame, frame);
}
