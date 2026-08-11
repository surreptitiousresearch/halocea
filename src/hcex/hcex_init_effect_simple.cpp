/* hcex_init_effect_simple @0x83683E00 — bridge helper: spawn an HCEX effect by name at a point with an
 * orientation vector, using a single default marker string.
 *
 * DEVIATION: the point/vector components are routed through doubles by the soft-float ABI; they are
 * floats copied into hcex_float3 buffers. */

#include "../headers/hcex/hcex_float3.h"
#include "../headers/real_point3d.h"
#include "../headers/real_vector3d.h"

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern "C" void hcex_create_effect(const char *eff_name, int obj_follow, int plr_idx, const hcex_float3 *position, const hcex_float3 *orientation, const char **marker, int npoints);

extern "C" void hcex_init_effect_simple(const char *eff_name, real_point3d *point, real_vector3d *vector)
{
    const char *marker = empty_string;
    hcex_float3 position;
    hcex_float3 orientation;
    position.x = point->n[0];
    position.y = point->n[1];
    position.z = point->n[2];
    orientation.x = vector->n[0];
    orientation.y = vector->n[1];
    orientation.z = vector->n[2];
    hcex_create_effect(eff_name, -1, -1, &position, &orientation, &marker, 1);
}
