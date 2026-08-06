/* halo_update_pelican_effects 0x83682348 — bridge shim: forward a pelican thruster effect update from the
 * Blam engine (real_point3d/real_vector3d) into the hcex engine (hcex_float3). Either spatial argument may
 * be null, in which case the corresponding hcex_float3 is left uninitialized (matching the original; the
 * receiver only reads the components it needs based on `start`). */

#include "../headers/real_point3d.h"
#include "../headers/real_vector3d.h"
#include "../headers/hcex/hcex_float3.h"

extern "C" void hcex_update_pelican_effects(int vehicle_index, const char *src_obj, hcex_float3 *point,
        hcex_float3 *vector, int start);

extern "C" void halo_update_pelican_effects(int vehicle_index, const char *src_obj, real_point3d *point,
        real_vector3d *vector, int start)
{
    hcex_float3 point_f3;
    hcex_float3 vector_f3;

    if ( point )
    {
        point_f3.x = point->n[0];
        point_f3.y = point->n[1];
        point_f3.z = point->n[2];
    }
    if ( vector )
    {
        vector_f3.x = vector->n[0];
        vector_f3.y = vector->n[1];
        vector_f3.z = vector->n[2];
    }

    hcex_update_pelican_effects(vehicle_index, src_obj, &point_f3, &vector_f3, start);
}
