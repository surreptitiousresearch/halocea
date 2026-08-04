#include <math.h>
#include "m3dMATR.h"

// m3dMATR::MakeEuler @ 0x82544CC0
// Build a rotation matrix from XYZ Euler angles given in degrees. The translation row is
// cleared and the matrix is made homogeneous (elements[15] = 1).
//
// NOTE: the decompiler flagged "local variable allocation has failed" and lost the first
// float argument (rendered it as an uninitialised temp); the mangle ?...@@QAAXMMM@Z and the
// per-angle degrees->radians scaling (constant 0.017453292 = PI/180) confirm three float
// inputs. cos/sin are evaluated per angle and mapped back from the scratch temporaries.
void m3dMATR::MakeEuler(float angleX, float angleY, float angleZ)
{
    const float DEG2RAD = 0.017453292f;

    float radX = angleX * DEG2RAD;
    float radY = angleY * DEG2RAD;
    float radZ = angleZ * DEG2RAD;

    float cosX = (float)cos(radX), sinX = (float)sin(radX);
    float cosY = (float)cos(radY), sinY = (float)sin(radY);
    float cosZ = (float)cos(radZ), sinZ = (float)sin(radZ);

    elements[0]  = cosZ * cosY;
    elements[1]  = sinY * sinX * cosZ + sinZ * cosX;
    elements[2]  = sinZ * sinX - sinY * cosX * cosZ;
    elements[3]  = 0.0f;

    elements[4]  = -(sinZ * cosY);
    elements[5]  = cosZ * cosX - sinY * sinX * sinZ;
    elements[6]  = sinY * cosX * sinZ + cosZ * sinX;
    elements[7]  = 0.0f;

    elements[8]  = sinY;
    elements[9]  = -(cosY * sinX);
    elements[10] = cosY * cosX;
    elements[11] = 0.0f;

    elements[12] = 0.0f;
    elements[13] = 0.0f;
    elements[14] = 0.0f;
    elements[15] = 1.0f;
}
