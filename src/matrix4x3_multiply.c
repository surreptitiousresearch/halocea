/* matrix4x3_multiply @0x83706910 — concatenate two 4x3 transforms (result = a * b). Thunk to the
 * VMX/AltiVec implementation. */

#include "headers/real_matrix4x3.h"

extern void matrix4x3_multiply_vmx(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);

void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result)
{
    matrix4x3_multiply_vmx(a, b, result);
}
