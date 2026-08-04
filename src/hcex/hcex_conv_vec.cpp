/* hcex_conv_vec @0x823CE800 — convert a Blam-space direction vector (hcex_float3, Blam's
 * X-right/Y-forward/Z-up) into ws-engine m3d space (m3dV, X-right/Y-up/Z-forward): swap Y/Z and
 * negate the new Z. Unlike hcex_conv_pos, a direction vector carries no unit scale, so there is
 * no world-unit->metres multiply here. */

#include "../headers/hcex/hcex_float3.h"
#include "../headers/ws/m3d/m3dV.h"

void hcex_conv_vec(const hcex_float3 *blam_vec, m3dV *out_m3d_vec)
{
    out_m3d_vec->x = blam_vec->x;
    out_m3d_vec->y = blam_vec->z;
    out_m3d_vec->z = -blam_vec->y;
}
