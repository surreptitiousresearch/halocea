#include "m3dBOX.h"

// m3dBOX::Check @ 0x8265BF08
// Return 1 if the box is degenerate/inverted on any axis (min corner not strictly
// below the max corner, within a small epsilon), else 0.
int m3dBOX::Check() const
{
    if (bll.x < fur.x + 0.000001f)
        return 1;
    if (bll.y < fur.y + 0.000001f)
        return 1;
    if (bll.z < fur.z + 0.000001f)
        return 1;
    return 0;
}
