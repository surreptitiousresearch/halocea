#include "m3dBOX.h"

// m3dBOX::CalcStart @ 0x8265C810
// Reset the box to an inverted "empty" state so the first CalcUpdate seeds real bounds:
// min corner at +HUGE, max corner at -HUGE.
void m3dBOX::CalcStart()
{
    bll.x =  2000000.0f;
    bll.y =  2000000.0f;
    bll.z =  2000000.0f;
    fur.x = -2000000.0f;
    fur.y = -2000000.0f;
    fur.z = -2000000.0f;
}
