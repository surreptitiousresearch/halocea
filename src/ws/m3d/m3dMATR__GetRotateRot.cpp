#include <math.h>
#include "m3dMATR.h"
#include "m3d_boundary.h"

// m3dMATR::GetRotateRot @ 0x82542D08
// Decompose this matrix's rotation into XYZ Euler angles, in DEGREES (each atan2/asin result is
// scaled by RAD2DEG = 57.295776). elements[2] is the sin of the Y (pitch) angle; when it is at
// (near) +/-1 the X and Z axes are gimbal-locked and a degenerate branch is used.
void m3dMATR::GetRotateRot(float *rotX, float *rotY, float *rotZ) const
{
    const float RAD2DEG = 57.295776f;

    float sinPitch = elements[2];

    if (fabsf(1.0f - fabsf(sinPitch)) <= 0.000001f)
    {
        // Gimbal lock: pitch is +/-90 degrees. Roll folds into yaw; pin X to zero.
        *rotY = (m3dSign(-sinPitch) * 1.5707964f) * RAD2DEG;
        *rotX = 0.0f;
        *rotZ = -((float)atan2(elements[4], elements[5]) * RAD2DEG);
    }
    else
    {
        *rotY = (float)asin(_m3dClamp(-1.0f, 1.0f, -sinPitch)) * RAD2DEG;
        *rotX = (float)atan2(elements[6], elements[10]) * RAD2DEG;
        *rotZ = (float)atan2(elements[1], elements[0]) * RAD2DEG;
    }
}
