#include <math.h>
#include "../headers/havok/hkRotation.h"
#include "../headers/havok/hkVector4.h"

/*
 * quaternionFromRotatation @ 0x838E1050
 *
 * Converts a 3x3 rotation matrix (column-major hkRotation) into a unit
 * quaternion (x,y,z,w). Classic Shoemake trace / largest-diagonal algorithm.
 *
 * The matrix is column-major: element m[row][col] lives at flat float index
 * (col*4 + row) because each hkVector4 column is padded to four floats. The
 * three diagonal elements are therefore at flat indices 0, 5 and 10.
 *
 * DEVIATION: the decompiler DROPPED the store of the w component in the
 * largest-diagonal branch (`stfs f5, var_34`); it has been restored here as
 * quaternion[3]. Verified against disassembly (0x838E1208 / 0x838E120C).
 * DEVIATION: the original wrote the {1,2,0} "next axis" table into the same
 * stack slots later used for the output quaternion (the compiler proved the
 * lifetimes disjoint). Reconstructed as a separate const table for clarity.
 * DEVIATION: the final 128-bit copy of the assembled quaternion into *quatOut
 * (lvx128/stvx128) is reconstructed as four scalar stores.
 */
void quaternionFromRotatation(hkVector4 *quatOut, const hkRotation *r)
{
    /* Flat, column-major view over the nine matrix elements. */
    const float *m = &r->m_col0.m_quad.___u0.v[0];

    float quaternion[4];
    float trace = m[0] + m[5] + m[10];

    if (trace > 0.0f)
    {
        float root = sqrtf(trace + 1.0f);
        float half_inv_root = 0.5f / root;

        quaternion[0] = (m[6] - m[9]) * half_inv_root;   /* x: m21 - m12 */
        quaternion[1] = (m[8] - m[2]) * half_inv_root;   /* y: m02 - m20 */
        quaternion[2] = (m[1] - m[4]) * half_inv_root;   /* z: m10 - m01 */
        quaternion[3] = root * 0.5f;                     /* w */
    }
    else
    {
        static const int next_axis[3] = { 1, 2, 0 };

        int largest = (m[5] > m[0]) ? 1 : 0;             /* start with larger of m00 / m11 */
        if (m[10] > m[5 * largest])                      /* then compare against m22 */
            largest = 2;

        int i = largest;
        int j = next_axis[i];
        int k = next_axis[j];

        float root = sqrtf(m[5 * i] - (m[5 * k] + m[5 * j]) + 1.0f);
        float half_inv_root = 0.5f / root;

        quaternion[i] = root * 0.5f;
        quaternion[3] = (m[4 * j + k] - m[4 * k + j]) * half_inv_root;   /* w: m[k][j] - m[j][k] */
        quaternion[j] = (m[4 * j + i] + m[4 * i + j]) * half_inv_root;   /* m[i][j] + m[j][i] */
        quaternion[k] = (m[4 * k + i] + m[4 * i + k]) * half_inv_root;   /* m[i][k] + m[k][i] */
    }

    quatOut->m_quad.___u0.v[0] = quaternion[0];
    quatOut->m_quad.___u0.v[1] = quaternion[1];
    quatOut->m_quad.___u0.v[2] = quaternion[2];
    quatOut->m_quad.___u0.v[3] = quaternion[3];
}
