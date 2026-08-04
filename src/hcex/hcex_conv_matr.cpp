/* hcex_conv_matr @0x83682F60 — hcex bridge: convert a Blam real_matrix4x3 transform into the HCEX
 * hcex_matr4x3 form. Both structs share the same layout (uniform scale + forward/left/up basis + position),
 * so this copies the four basis/position vectors straight across. The uniform scale field is intentionally
 * not copied (the Blam->ws bridge uses only the orientation and translation here).
 *
 * DEVIATION: the decompile spelled the forward row as `*(hcex_float3 *)&in->n[0][0]`; that is exactly
 * `in->forward` (row 0 at offset 0x04), written as the named field here. */

#include "../headers/real_matrix4x3.h"
#include "../headers/hcex/hcex_matr4x3.h"

extern "C" void hcex_conv_matr(real_matrix4x3 *in, hcex_matr4x3 *out)
{
    out->pos = *(hcex_float3 *)&in->position;
    out->forward = *(hcex_float3 *)&in->forward;
    out->left = *(hcex_float3 *)&in->left;
    out->up = *(hcex_float3 *)&in->up;
}
