/* action_vehicle_find_destination @0x83821AD0 — picks a ground-level destination point (and its collision
 * surface index) near `vehicle_index` for the actor to walk to before boarding, starting from the entry
 * point/hint already computed by action_vehicle_evaluate_seat (or action_vehicle_find_impromptu_seat). Anchors
 * on entry_point or hint_point (whichever is closer to the vehicle's bounding-sphere center, refined by a couple
 * of geometric sanity checks), offsets perpendicular to the vehicle at roughly `approach_distance * 1.1`, nudges
 * that point again to keep it from crowding the actor's own current position, then drops straight down onto the
 * collision BSP to find solid ground. `*ignore_hint_reference` is the in/out byte flag recording whether
 * entry_point (rather than the hint point) was used as the anchor.
 *
 * DEVIATION (signature): the DB's own applied prototype is one parameter too long from slot 4 onward — it
 * inserts a phantom `real_point3d *entry_facing` at r6 and shifts every later name one register right, which is
 * why r10 (its 8th param) is never read at all. The physical registers are r6 = the DB's `hint_point`, r7 = the
 * DB's `ignore_hint_reference`, r8 = the DB's `destination_point`, r9 = the DB's `surface_index_reference`;
 * r10 dead. Corroborated at every level: r6 is only ever LOADED from as three floats (`lfs 0/4/8(r6)`
 * @0x83821BE8-0x83821BF0 and the `mr r11, r6` anchor select @0x83821C44), so it is a read-only point, not a
 * facing vector; r7 is a byte in/out (`lbz r9, 0(r31)` @0x83821B68, `stb r29, 0(r31)` @0x83821E74), which is
 * `uint8_t *`, not the DB's `float *`; and all three call sites hand r6 the buffer the seat evaluator filled as
 * its `hint_point` out-parameter, never the entry_facing buffer — `addi r6, r1, 0xE0+var_70` @0x83822078 is
 * action_vehicle_evaluate_seat's r9 slot @0x83822030, `addi r6, r1, 0x100+var_68` @0x83822320 is its r9 slot
 * @0x83822254, and `addi r6, r1, 0xC0+var_70` @0x838226B8 is action_vehicle_find_impromptu_seat's r7 slot
 * @0x83822658. `ignore_hint_reference` is confirmed independently by vehicle_state_data's own
 * `uint8_t ignore_hint` at +0x07, whose address action_vehicle_perform passes there (`addi r7, r31, 7`
 * @0x8382231C); the other two call sites pass NULL. The unused r10 slot
 * matches the identical unset-trailing-register pattern already noted for action_flee_find_flee_position.
 * Declared here with the corrected 7-param signature.
 *
 * DEVIATION (body): the decompiler mis-attributes the `collision_bsp_test_vector` result-struct argument to
 * `(collision_bsp_test_vector_result *)global_down3d` — disasm shows the real 8th argument is `&result`, a
 * plain stack local (`addi r10, r1, var_490`); `maximum_t` (a float, arg 7) consumes r9's GPR shadow slot,
 * shifting the decompiler's read of r10 into unrelated literal-pool bookkeeping. The two locals it does surface
 * from that struct (`v66`/`v67`) are `result.t` and `result.surface_index` at their real offsets (0x0/0x8),
 * confirmed against collision_bsp_test_vector_result's known layout.
 *
 * The vehicle object record ("vehicle") offsets 160/172 line up exactly with object_datum's own
 * bounding_sphere_center/bounding_sphere_radius fields (accounting for the 4-byte definition_index prefix), so
 * those are named directly. The vehicle's tag *definition* fields are typed via vehicle_definition.h:
 * +0x17C = unit.flags, +0x280 = unit.ai_vehicle_avoidance_radius. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/unit_definition_flags.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/object_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector2d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_bsp.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/collision_bsp_test_flags.h"
#include "headers/blam_data_globals.h"


extern float __fsqrts(float x);
extern float normalize2d(real_vector2d *v);
extern real_vector2d *perpendicular2d(const real_vector2d *a, real_vector2d *result);
extern uint8_t collision_bsp_test_vector(unsigned int flags, const struct collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *point, const real_vector3d *vector, float maximum_t, collision_bsp_test_vector_result *result);

uint8_t action_vehicle_find_destination(int actor_index, int vehicle_index, real_point3d *entry_point, const real_point3d *hint_point, uint8_t *ignore_hint_reference, real_point3d *destination_point, int *surface_index_reference)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    real_point3d fallback_point = *entry_point;

    object_datum *vehicle = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, vehicle->definition_index);

    uint8_t use_entry_point = ignore_hint_reference ? *ignore_hint_reference : 0;

    real_point3d chosen_point;

    if ( (vehicle_def->unit.flags & (1u << _unit_definition_entrance_inside_bounding_sphere_bit)) == 0 ) /* unit.flags +0x17C */
    {
        real_point3d marker = vehicle->object.bounding_sphere_center;

        float approach_distance = vehicle_def->unit.ai_vehicle_avoidance_radius; /* unit +0x104 (abs +640) */
        if ( approach_distance <= 0.0f )
            approach_distance = vehicle->object.bounding_sphere_radius;

        if ( !use_entry_point )
        {
            float marker_distance = __fsqrts(
                    (hint_point->n[0] - marker.n[0]) * (hint_point->n[0] - marker.n[0])
                  + (hint_point->n[2] - marker.n[2]) * (hint_point->n[2] - marker.n[2])
                  + (hint_point->n[1] - marker.n[1]) * (hint_point->n[1] - marker.n[1]));
            if ( marker_distance >= 0.5f )
            {
                if ( approach_distance <= marker_distance + 0.3f )
                    approach_distance = marker_distance + 0.3f;
            }
            else
            {
                use_entry_point = 1;
            }
        }

        const real_point3d *anchor = use_entry_point ? entry_point : hint_point;
        real_point3d anchor_point = *anchor;

        real_vector2d entry_direction;
        entry_direction.n[0] = anchor_point.n[0] - actor->input.position.body_position.n[0];
        entry_direction.n[1] = anchor_point.n[1] - actor->input.position.body_position.n[1];

        real_vector2d anchor_to_marker;
        anchor_to_marker.n[0] = marker.n[0] - anchor_point.n[0];
        anchor_to_marker.n[1] = marker.n[1] - anchor_point.n[1];

        if ( !use_entry_point )
        {
            /* if the hint point sits close enough to the entry_point->actor line, prefer entry_point as anchor */
            float dx = hint_point->n[0] - entry_point->n[0];
            float dy = hint_point->n[1] - entry_point->n[1];
            /* DEVIATION: was dx*ex - dy*ey; binary negates the full dot product — fmuls f4,f8,f0 (dx*ex)
             * @0x83821CC8 then fnmadds f3,f5,f13,f4 = -(dy*ey + dx*ex) @0x83821CCC. */
            float t = -(dx * entry_direction.n[0] + dy * entry_direction.n[1]);
            float px = dx * t + entry_direction.n[0];
            float py = dy * t + entry_direction.n[1];
            if ( py * py + px * px < 0.1225f )
                use_entry_point = 1;
        }

        if ( entry_direction.n[1] * entry_direction.n[1] + entry_direction.n[0] * entry_direction.n[0] > 0.0f )
        {
            uint8_t refined = 0;
            real_vector2d perp;

            float dot = entry_direction.n[1] * (marker.n[1] - actor->input.position.body_position.n[1])
                      + entry_direction.n[0] * (marker.n[0] - actor->input.position.body_position.n[0]);
            float t = dot / (entry_direction.n[1] * entry_direction.n[1] + entry_direction.n[0] * entry_direction.n[0]);

            if ( t > 0.0f && t < 1.2f )
            {
                perpendicular2d(&entry_direction, &perp);
                if ( perp.n[1] * (marker.n[1] - actor->input.position.body_position.n[1])
                   + perp.n[0] * (marker.n[0] - actor->input.position.body_position.n[0]) > 0.0f )
                {
                    perp.n[0] = -perp.n[0];
                    perp.n[1] = -perp.n[1];
                }
                refined = 1;
            }
            else if ( !use_entry_point )
            {
                perp.n[0] = -anchor_to_marker.n[0];
                perp.n[1] = -anchor_to_marker.n[1];
                refined = 1;
            }

            if ( refined && normalize2d(&perp) > 0.0f )
            {
                chosen_point.n[2] = fallback_point.n[2];
                chosen_point.n[1] = marker.n[1] + perp.n[1] * (approach_distance * 1.1f);
                chosen_point.n[0] = marker.n[0] + perp.n[0] * (approach_distance * 1.1f);

                float rx = chosen_point.n[0] - actor->input.position.body_position.n[0];
                float ry = chosen_point.n[1] - actor->input.position.body_position.n[1];
                float rz = fallback_point.n[2] - actor->input.position.body_position.n[2];
                float r2 = rx * rx + rz * rz + ry * ry;
                if ( r2 > 0.0001f && r2 < 4.0f )
                {
                    float r = __fsqrts(r2);
                    real_vector2d perp2;
                    perpendicular2d(&anchor_to_marker, &perp2);
                    if ( perp2.n[1] * ry + perp2.n[0] * rx < 0.0f )
                    {
                        perp2.n[0] = -perp2.n[0];
                        perp2.n[1] = -perp2.n[1];
                    }
                    if ( normalize2d(&perp2) > 0.0f )
                    {
                        /* z term's companion factor is a constant 0.0 in the original -- chosen_point.n[2]
                         * is unaffected here */
                        chosen_point.n[0] += perp2.n[0] * (2.0f - r);
                        chosen_point.n[1] += perp2.n[1] * (2.0f - r);
                    }
                }

                goto have_chosen_point;
            }
        }
    }

    chosen_point = fallback_point;

have_chosen_point:
    if ( ignore_hint_reference )
        *ignore_hint_reference = use_entry_point;

    real_point3d test_point_above;
    test_point_above.n[0] = global_up3d->n[0] + chosen_point.n[0];
    test_point_above.n[1] = global_up3d->n[1] + chosen_point.n[1];
    test_point_above.n[2] = global_up3d->n[2] + chosen_point.n[2];

    real_vector3d down_ray;
    down_ray.n[0] = global_down3d->n[0] * 4.0f;
    down_ray.n[1] = global_down3d->n[1] * 4.0f;
    down_ray.n[2] = global_down3d->n[2] * 4.0f;

    collision_bsp_test_vector_result result;
    if ( !collision_bsp_test_vector((1u << _collision_bsp_test_front_facing_surfaces_bit), global_collision_bsp, 0, 0, &test_point_above, &down_ray, 3.4028235e38f, &result) )
        return 0;

    destination_point->n[0] = down_ray.n[0] * result.t + test_point_above.n[0];
    destination_point->n[1] = down_ray.n[1] * result.t + test_point_above.n[1];
    destination_point->n[2] = down_ray.n[2] * result.t + test_point_above.n[2];
    *surface_index_reference = result.surface_index;
    return 1;
}
