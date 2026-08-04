#include "m3dMATR.h"

// m3dMATR::Identity @ 0x825429C8
// Set the matrix to identity. The compiler zeroed the 12 off-diagonal elements via
// six 64-bit (paired-float) stores through the union; reconstructed here as the
// equivalent per-element assignments, then the four diagonal elements set to 1.
void m3dMATR::Identity()
{
    elements[1]  = 0.0f; elements[2]  = 0.0f; elements[3]  = 0.0f;
    elements[4]  = 0.0f; elements[6]  = 0.0f; elements[7]  = 0.0f;
    elements[8]  = 0.0f; elements[9]  = 0.0f; elements[11] = 0.0f;
    elements[12] = 0.0f; elements[13] = 0.0f; elements[14] = 0.0f;

    elements[0]  = 1.0f;
    elements[5]  = 1.0f;
    elements[10] = 1.0f;
    elements[15] = 1.0f;
}
