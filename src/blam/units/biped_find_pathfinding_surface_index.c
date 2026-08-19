/* biped_find_pathfinding_surface_index @0x837AD338 — resolve the pathfinding (floor) surface under a biped,
 * caching the result in the biped datum (pathfinding_surface_index / pathfinding_point /
 * support_surface_index / last_pathfinding_attempt_time / last_pathfinding_surface_index). Flying/non-grounded biped types (definition flag 0x4 set,
 * object flag 0x4 clear) skip pathfinding and just report the raw origin. Otherwise, when the cache is
 * invalid and the throttle has elapsed, the previous/last surface is re-projected (or the previous surface
 * re-tested); failing that, a downward ground probe (biped_find_ground_surface) finds the surface. The
 * resolved point is written to pathfinding_point and the surface index returned (-1 on miss).
 *
 * DEVIATION: the biped_find_ground_surface call is mis-rendered by the FPR-shadow ABI — the float
 * ground_distance reserves a GPR slot, so the decompiler invents a phantom 3rd arg (v10) and shifts the
 * rest. The real call (from disasm) is (biped_index, 2.0, global_down3d, projected_point, nullptr). */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/data_array.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_damage_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern int game_time_get(void);
extern uint8_t collision_surface_test_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *point);
extern real_point3d *collision_surface_project_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *p2d, real_point3d *p3d);
extern int collision_surface_find_closest_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *point, real_point2d *result);
extern int biped_find_ground_surface(int biped_index, float ground_distance, const real_vector3d *ground_direction, real_point3d *surface_point, real_vector3d *surface_normal);

int biped_find_pathfinding_surface_index(int biped_index, real_point3d *pathfinding_point)
{
    biped_datum *biped = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;
    biped_definition *definition = TAG_GET(biped_definition, biped->definition_index);

    if ((definition->biped.flags & (1u << _biped_flying_bit)) != 0 && (biped->object.damage_flags & (1u << _object_dead_bit)) == 0)
    {
        biped->biped.pathfinding_surface_index = -1;
        object_get_origin(biped_index, pathfinding_point);
    }
    else if (biped->biped.pathfinding_surface_index == -1)
    {
        int now = game_time_get();
        if (now > biped->biped.last_pathfinding_attempt_time)
        {
            const collision_bsp *bsp = global_collision_bsp;
            real_point3d projected_point[6];
            projected_point[0].n[0] = biped->biped.pathfinding_point.n[0];
            projected_point[0].n[1] = biped->biped.pathfinding_point.n[1];
            projected_point[0].n[2] = biped->biped.pathfinding_point.n[2];
            int last_surface = biped->biped.support_surface_index;
            biped->biped.last_pathfinding_attempt_time = now;

            if (last_surface == -1)
            {
                int previous_surface = biped->biped.last_pathfinding_surface_index;
                /* KEEP: real_point3d -> real_point2d reinterpret; the surface routines consume only the
                 * (x,y) prefix, so the 3d point's first two floats are the 2d point. */
                if (previous_surface != -1
                    && collision_surface_test_point2d(bsp, previous_surface, 2, 1u,
                                                      (const real_point2d *)&biped->biped.pathfinding_point))
                {
                    biped->biped.pathfinding_surface_index = previous_surface;
                    collision_surface_project_point2d(bsp, previous_surface, 2, 1u,
                                                      (const real_point2d *)&biped->biped.pathfinding_point, projected_point);
                }
            }
            else
            {
                real_point2d closest_point;
                collision_surface_find_closest_point2d(bsp, last_surface, 2, 1u,
                                                       (const real_point2d *)&biped->biped.pathfinding_point, &closest_point);
                collision_surface_project_point2d(bsp, last_surface, 2, 1u, &closest_point, projected_point);
                biped->biped.pathfinding_surface_index = last_surface;
            }

            if (biped->biped.pathfinding_surface_index == -1)
                biped->biped.pathfinding_surface_index = biped_find_ground_surface(biped_index, 2.0f, global_down3d,
                                                                   projected_point, nullptr);

            if (biped->biped.pathfinding_surface_index != -1)
            {
                biped->biped.pathfinding_point.n[0] = projected_point[0].n[0];
                biped->biped.pathfinding_point.n[1] = projected_point[0].n[1];
                biped->biped.pathfinding_point.n[2] = projected_point[0].n[2];
                biped->biped.last_pathfinding_surface_index = biped->biped.pathfinding_surface_index;
            }
        }
    }

    pathfinding_point->n[0] = biped->biped.pathfinding_point.n[0];
    pathfinding_point->n[1] = biped->biped.pathfinding_point.n[1];
    pathfinding_point->n[2] = biped->biped.pathfinding_point.n[2];
    return biped->biped.pathfinding_surface_index;
}
