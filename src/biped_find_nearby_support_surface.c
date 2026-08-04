/* biped_find_nearby_support_surface @0x837B0438 — find the collision surface the biped is most likely resting
 * on and cache it on the object. It queries the global collision BSP with a sphere sized to the biped's physics
 * pill (radius + 0.05), then over every returned surface computes the biped centre's signed distance to that
 * surface's (facing-corrected) plane and keeps the surface whose plane is closest below/around the centre. The
 * winning surface index is stored at object+1240 and its plane (normal + distance) is written both to the
 * short-form support-normal slot (object+128) and the full support-plane slot (object+1300).
 *
 * DEVIATION: register-pun doubles are plain float math. The sphere-test result buffer (Hex-Rays rendered its
 * count/index array as free-standing locals with a lost pointer) is the &result passed to collision_bsp_test_sphere
 * per the call-site disasm; surface plane designator negation follows the standard collision facing-bit idiom
 * (16*plane wraps mod 2^32 so the sign only selects negation). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/collision_bsp.h"
#include "headers/collision_bsp_test_sphere_result.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/blam_data_globals.h"


extern void biped_get_physics_pill(int biped_index, real_point3d *base, float *height, float *width);
extern const uint8_t *breakable_surface_flags_get(void);
extern uint8_t collision_bsp_test_sphere(const collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *center, float radius, collision_bsp_test_sphere_result *result);

void biped_find_nearby_support_surface(int biped_index)
{
    collision_bsp *bsp = global_collision_bsp;
    biped_datum *biped =
        (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;

    real_point3d centre[2];
    float pill_axis[4];
    float pill_radius[2];
    biped_get_physics_pill(biped_index, centre, pill_axis, pill_radius);

    const unsigned __int8 *breakable_surface_flags = breakable_surface_flags_get();

    collision_bsp_test_sphere_result result;
    if ( !collision_bsp_test_sphere(bsp, 256, breakable_surface_flags, centre,
            pill_radius[0] + 0.050000001f, &result) )
        return;

    int best_surface = -1;
    float best_distance = 3.4028235e38f;
    float best_plane[4];

    for ( int i = 0; i < result.surface_count; i++ )
    {
        int surface_index = result.surface_indices[i];
        int plane_designator = ((collision_surface *)bsp->surfaces.address)[surface_index].plane_designator;
        const float *plane = (const float *)&((const real_plane3d *)bsp->bsp3d.planes.address)[plane_designator];

        float nx, ny, nz, d;
        if ( plane_designator >= 0 )
        {
            nx = plane[0];  ny = plane[1];  nz = plane[2];  d = plane[3];
        }
        else
        {
            nx = -plane[0]; ny = -plane[1]; nz = -plane[2]; d = -plane[3];
        }

        float signed_distance = (centre[0].n[1] * ny + (nx * centre[0].n[0] + centre[0].n[2] * nz)) - d;
        if ( signed_distance < best_distance )
        {
            best_distance = signed_distance;
            best_surface = surface_index;
            best_plane[0] = nx;
            best_plane[1] = ny;
            best_plane[2] = nz;
            best_plane[3] = d;
        }
    }

    if ( best_surface != -1 )
    {
        biped->biped.support_surface_index = best_surface;
        biped->object.up.n[0] = best_plane[0];              /* support normal */
        biped->biped.ground_plane.n.n[0] = best_plane[0];  /* full support plane */
        biped->biped.ground_plane.n.n[1] = best_plane[1];
        biped->biped.ground_plane.n.n[2] = best_plane[2];
        biped->biped.ground_plane.d = best_plane[3];
        biped->object.up.n[1] = best_plane[1];
        biped->object.up.n[2] = best_plane[2];
    }
}
