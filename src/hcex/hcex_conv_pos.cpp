/* hcex_conv_pos @0x823CE7C8 — convert a Blam-space position (hcex_float3, Blam world units,
 * Blam's X-right/Y-forward/Z-up) into ws-engine m3d space (m3dV, metres, X-right/Y-up/Z-forward):
 * swap Y/Z, negate the new Z, and scale world-units->metres (Halo's world unit is defined as
 * 10 feet == 3.048 m, matching the compiled constant, reproduced verbatim). */

#include "../headers/hcex/hcex_float3.h"
#include "../headers/ws/m3d/m3dV.h"

void hcex_conv_pos(const hcex_float3 *blam_pos, m3dV *out_m3d_pos)
{
    out_m3d_pos->x = blam_pos->x * 3.0480001f;
    out_m3d_pos->y = blam_pos->z * 3.0480001f;
    out_m3d_pos->z = blam_pos->y * -3.0480001f;
}
