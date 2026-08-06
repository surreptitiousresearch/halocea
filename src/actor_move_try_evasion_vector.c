/* actor_move_try_evasion_vector @0x837C7428 — test whether an actor can evade along a 2D direction, either as a
 * flat dodge or a ledge hop. It first projects the actor's pathfinding point out by evade_distance along
 * evasion_vector and runs structure_test_pill2d there; if that path is clear it accepts, unless the destination's
 * height differs from the actor's by more than half evade_distance (a step up always rejects; a step down rejects
 * only when ledge hops are disabled). If the flat dodge failed and ledge hops are allowed (maximum_ledge_height >
 * 0), it casts a collision ray from the actor's mid-body out along the evade direction; a clear ray marks a ledge
 * and, when the ledge height is bounded, a second downward ray of maximum_ledge_height confirms there is ground to
 * land on. Returns whether an evade was found and writes the is-ledge flag through evasion_is_ledge_reference.
 *
 * DEVIATION: FPR-shadow ABI trap — the float args evade_distance (f1) and maximum_ledge_height (f2) burn the
 * r5/r6 GPR shadow slots, so Hex-Rays invented two phantom params and pushed the real evasion_is_ledge_reference
 * (r7) and result (r8) out into those phantom slots; the DB's 6-arg prototype is authoritative. It also corrupted the
 * structure_test_pill2d call: disasm (0x837C74C0-0x837C74E4) shows flags = 0 and result = this function's own
 * `result` param (r28, spilled to the stack arg slot), not the nullptr/garbage Hex-Rays emitted. Both
 * collision_bsp_test_vector calls write into one local scratch result (r1+var_470); Hex-Rays misresolved that
 * stack address to a global `g_PFP[208]` for the first call. Both use maximum_t = FLT_MAX (f31 is reloaded to
 * 0x7F7FFFFF at 0x837C7584 after serving as evade_distance for the point projection). */

#include <stdint.h>
#include "headers/actor_definition.h"
#include "headers/real_vector2d.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/path_collision_result.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/structure_bsp.h"
#include "headers/collision_bsp.h"
#include "headers/collision_test_flags.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


#include "headers/structure_bsp.h"
extern void actor_find_pathfinding_location(uint16_t actor_index);
extern uint8_t structure_test_pill2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces, const real_point2d *start_point, int start_surface_index, const real_point2d *end_point, int end_surface_index, float radius, unsigned int flags, path_collision_result *result);
extern uint8_t collision_bsp_test_vector(unsigned int flags, const struct collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, float maximum_t, collision_bsp_test_vector_result *result);

uint8_t actor_move_try_evasion_vector(int actor_index, real_vector2d *evasion_vector, float evade_distance, float maximum_ledge_height, uint8_t *evasion_is_ledge_reference, path_collision_result *result)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    uint8_t found = 0;
    uint8_t is_ledge = 0;
    const actor_definition *definition = TAG_GET(const actor_definition, actor->meta.definition_index);

    if ( !actor->state.flying )
    {
        real_point2d target_point;
        target_point.n[0] = evasion_vector->n[0] * evade_distance + actor->input.position.body_position.n[0];
        target_point.n[1] = evasion_vector->n[1] * evade_distance + actor->input.position.body_position.n[1];

        actor_find_pathfinding_location(actor_index);

        if ( !structure_test_pill2d(global_structure_bsp, actor->emotions.ignorant_of_broken_surfaces,
                (const real_point2d *)&actor->input.pathfinding_point, actor->input.pathfinding_surface_index,
                &target_point, -1,
                definition->moving.pathfinding_radius, 0, result) )
        {
            found = 1;
            float height_delta = result->point.n[2] - actor->input.position.body_position.n[2];
            if ( height_delta > evade_distance * 0.5f
                    || (maximum_ledge_height == 0.0f && height_delta < evade_distance * -0.5f) )
                found = 0;
        }

        if ( !found && maximum_ledge_height > 0.0f )
        {
            collision_bsp_test_vector_result test_result;
            real_vector3d evade_ray;
            real_point3d body_center;

            evade_ray.n[0] = evasion_vector->n[0] * evade_distance;
            evade_ray.n[1] = evasion_vector->n[1] * evade_distance;
            evade_ray.n[2] = 0.0f;
            body_center.n[0] = (actor->input.position.head_position.n[0] + actor->input.position.body_position.n[0]) * 0.5f;
            body_center.n[1] = (actor->input.position.head_position.n[1] + actor->input.position.body_position.n[1]) * 0.5f;
            body_center.n[2] = (actor->input.position.body_position.n[2] + actor->input.position.head_position.n[2]) * 0.5f;

            if ( !collision_bsp_test_vector(
                    (1u << _collision_test_front_facing_surfaces_bit) | (1u << _collision_test_back_facing_surfaces_bit),
                    global_collision_bsp, 0, nullptr, &body_center, &evade_ray,
                    3.4028235e38f, &test_result) )
            {
                found = 1;
                is_ledge = 1;
                if ( maximum_ledge_height < 3.4028235e38f )
                {
                    real_point3d ledge_edge;
                    real_vector3d drop_ray;

                    ledge_edge.n[0] = evade_ray.n[0] + body_center.n[0];
                    ledge_edge.n[1] = evade_ray.n[1] + body_center.n[1];
                    ledge_edge.n[2] = body_center.n[2] + evade_ray.n[2];
                    drop_ray.n[0] = global_down3d->n[0] * maximum_ledge_height;
                    drop_ray.n[1] = global_down3d->n[1] * maximum_ledge_height;
                    drop_ray.n[2] = global_down3d->n[2] * maximum_ledge_height;

                    found = collision_bsp_test_vector(
                            (1u << _collision_test_front_facing_surfaces_bit) | (1u << _collision_test_back_facing_surfaces_bit),
                            global_collision_bsp, 0, nullptr, &ledge_edge,
                            &drop_ray, 3.4028235e38f, &test_result) != 0;
                }
            }
        }
    }

    if ( evasion_is_ledge_reference )
        *evasion_is_ledge_reference = is_ledge;
    return found;
}
