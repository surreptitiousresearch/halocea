#include "../../headers/ws/ai/aiCMD_GOAL.h"

// m3d subsystem — squared vector length. boundary.
extern float m3dLengthVector_2(const m3dV *v);

// aiCMD_GOAL::DetectBigTurn(const m3dV&) @ 0x83175E08
// ?DetectBigTurn@aiCMD_GOAL@@IAAHABUm3dV@@@Z  (protected)
// Nonzero when `newDir` differs enough from the current goal direction to count as a sharp turn.
// Degenerate (near-zero) directions never count. Otherwise, a turn is "big" when the cosine of
// the angle between the two directions (their dot product, both assumed unit-length) falls below
// the configured threshold dirCosMaxAngle.
int aiCMD_GOAL::DetectBigTurn(const m3dV &newDir)
{
    if (m3dLengthVector_2(&dirGoal) < 0.000001f)
        return 0;
    if (m3dLengthVector_2(&newDir) < 0.000001f)
        return 0;

    float dot = newDir.x * dirGoal.x + (dirGoal.z * newDir.z + dirGoal.y * newDir.y);
    if (dot >= dirCosMaxAngle)
        return 0;
    return 1;
}
