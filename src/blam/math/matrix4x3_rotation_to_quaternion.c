/* matrix4x3_rotation_to_quaternion @0x83705BE8 — convert a rotation matrix to a quaternion (Shepperd's method).
 * If the trace is positive, w is the dominant term; otherwise the largest diagonal element selects which vector
 * component dominates, and the cyclic index table next_field = {1,2,0} walks the remaining axes. n[3]=w. */

#include <math.h>
#include "headers/real_matrix4x3.h"
#include "headers/real_quaternion.h"
#include "headers/blam_data_globals.h"


void matrix4x3_rotation_to_quaternion(const real_matrix4x3 *matrix, real_quaternion *quaternion)
{
    float trace = (matrix->n[0][0] + matrix->n[1][1]) + matrix->n[2][2];
    if ( trace > 0.0f )
    {
        float root = sqrtf(trace + 1.0f);
        quaternion->w = root * 0.5f;
        float half_inverse = 0.5f / root;
        quaternion->v.n[0] = (matrix->n[2][1] - matrix->n[1][2]) * half_inverse;
        quaternion->v.n[1] = (matrix->n[0][2] - matrix->n[2][0]) * half_inverse;
        quaternion->v.n[2] = (matrix->n[1][0] - matrix->n[0][1]) * half_inverse;
    }
    else
    {
        int i = (matrix->n[1][1] > matrix->n[0][0]) ? 1 : 0;
        if ( matrix->n[2][2] > matrix->n[i][i] )
            i = 2;
        int j = next_field[i];
        int k = next_field[j];

        float components[3];
        float root = sqrtf((matrix->n[i][i] - (matrix->n[k][k] + matrix->n[j][j])) + 1.0f);
        components[i] = root * 0.5f;
        float half_inverse = (root != 0.0f) ? (0.5f / root) : root;
        components[j] = (matrix->n[i][j] + matrix->n[j][i]) * half_inverse;
        components[k] = (matrix->n[i][k] + matrix->n[k][i]) * half_inverse;

        quaternion->v.n[2] = components[2];
        quaternion->v.n[1] = components[1];
        quaternion->v.n[0] = components[0];
        quaternion->w = (matrix->n[k][j] - matrix->n[j][k]) * half_inverse;
    }
}
