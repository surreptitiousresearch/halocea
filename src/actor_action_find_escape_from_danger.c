/* actor_action_find_escape_from_danger @0x837F2698 — pick the better of two perpendicular escape directions
 * for an actor fleeing an immediate danger (e.g. a grenade). Starting from the actor's heading (its velocity
 * direction, or if nearly stationary its facing / position-derived direction / global_forward2d), it forms the
 * two sideways escape directions (left and right perpendicular) and probes each with
 * actor_move_try_evasion_vector out to the unit's escape distance (unit definition float @0x238). Each probe
 * yields whether an evade path exists, whether it is a ledge drop, and how far the resulting point lies from
 * the danger movement line. It then chooses:
 *   - only one side viable            → that side (direction code 0 = left, 1 = right);
 *   - both viable, clearly asymmetric → the safer side (code 0);
 *   - both viable and roughly equal   → code 4 (either/aligned);
 *   - neither viable                  → code -1.
 * Writes the chosen direction code, the escape distance, the ledge flag, and the escape heading through the
 * out-params, and returns whether the chosen escape actually clears the danger.
 *
 * DEVIATIONS (disasm-resolved): actor_move_try_evasion_vector is FPR-shadow'd (its two float args burn GPR
 * slots) — the decompiler rendered the trailing out-params as a 0x82000000 pun and an actor-unit-index
 * pointer; disasm at 0x837F28E4/0x837F293C shows evasion_is_ledge_reference = &ledge_left/&ledge_right (stack
 * bytes) and result = a single shared stack path_collision_result. All actor fields are raw offsets into the
 * opaque behaviour payload per project convention. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_definition_flags.h"
#include "headers/path_collision_result.h"
#include "headers/real_vector2d.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern float normalize2d(real_vector2d *v);
extern float __fsqrts(float x);
extern float __fabs(float x);
extern uint8_t actor_move_try_evasion_vector(int actor_index, real_vector2d *evasion_vector, float evade_distance, float maximum_ledge_height, uint8_t *evasion_is_ledge_reference, path_collision_result *result);
extern float point_to_line_distance_squared3d(const real_point3d *point, const real_point3d *base, const real_vector3d *height);


int actor_action_find_escape_from_danger(int actor_index, int16_t *escape_direction_reference, float *escape_distance_reference, real_vector2d *alignment_vector_reference, uint8_t *escape_is_ledge_reference)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int unit_index = actor->meta.unit_index;

    int result = 0;
    unsigned __int8 escape_is_ledge = 0;
    __int16 escape_direction = -1;

    /* Escape distance from the unit's definition tag (float @0x238). */
    float escape_distance =
            TAG_GET(float, *(int *)((char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum))[142];

    /* Written through to alignment_vector_reference even when escape_distance <= 0, in which case it is read
     * uninitialized — faithful to the shipped code (v41 has no init on that path). */
    real_vector2d escape_heading;
    if ( escape_distance > 0.0f )
    {
        /* Heading = velocity direction; if nearly stationary, fall back to facing / position / forward. */
        float neg_vx = -actor->danger_zone.velocity.n[0];
        float neg_vy = -actor->danger_zone.velocity.n[1];
        float speed = __fsqrts(((neg_vy * neg_vy) + (neg_vx * neg_vx)));

        float heading_x, heading_y;
        if ( __fabs(speed) < 0.000099999997f || speed < SECONDS_PER_TICK )
        {
            escape_heading.n[0] = actor->danger_zone.position.n[0] - actor->input.position.body_position.n[0];
            escape_heading.n[1] = actor->danger_zone.position.n[1] - actor->input.position.body_position.n[1];
            if ( normalize2d(&escape_heading) == 0.0f )
            {
                escape_heading = *(real_vector2d *)&actor->input.facing_vector;   /* facing */
                if ( normalize2d(&escape_heading) == 0.0f )
                    escape_heading = *global_forward2d;
            }
            heading_x = escape_heading.n[0];
            heading_y = escape_heading.n[1];
        }
        else
        {
            heading_x = ((1.0f / speed) * neg_vx);
            escape_heading.n[0] = heading_x;
            heading_y = ((1.0f / speed) * neg_vy);
            escape_heading.n[1] = heading_y;
        }

        /* Ledge tolerance: 8.0 for actors flagged for ledge-drops (_actor_definition_dive_off_ledges_bit), else 0. */
        float max_ledge_height =
                (*TAG_GET(int, actor->meta.definition_index) & (1u << _actor_definition_dive_off_ledges_bit)) != 0 ? 8.0f : 0.0f;

        /* Danger movement line direction. */
        real_vector3d danger_movement;
        danger_movement.n[0] = actor->danger_zone.predict_danger_position.n[0] - actor->danger_zone.position.n[0];
        danger_movement.n[1] = actor->danger_zone.predict_danger_position.n[1] - actor->danger_zone.position.n[1];
        danger_movement.n[2] = actor->danger_zone.predict_danger_position.n[2] - actor->danger_zone.position.n[2];

        real_vector2d perp_left;
        perp_left.n[0] = -heading_y;
        perp_left.n[1] = heading_x;
        real_vector2d perp_right;
        perp_right.n[0] = heading_y;
        perp_right.n[1] = -heading_x;

        /* Probe points at escape_distance along each perpendicular (z unchanged). */
        real_point3d probe_left;
        probe_left.n[0] = (perp_left.n[0] * escape_distance) + actor->input.position.body_position.n[0];
        probe_left.n[1] = (heading_x * escape_distance) + actor->input.position.body_position.n[1];
        probe_left.n[2] = (escape_distance * 0.0f) + actor->input.position.body_position.n[2];
        real_point3d probe_right;
        probe_right.n[0] = (heading_y * escape_distance) + actor->input.position.body_position.n[0];
        probe_right.n[1] = (perp_right.n[1] * escape_distance) + actor->input.position.body_position.n[1];
        probe_right.n[2] = (escape_distance * 0.0f) + actor->input.position.body_position.n[2];

        unsigned __int8 ledge_left = 0;
        unsigned __int8 ledge_right = 0;
        path_collision_result probe_result;

        unsigned __int8 found_left = actor_move_try_evasion_vector(actor_index, &perp_left, escape_distance,
                max_ledge_height, &ledge_left, &probe_result);
        float dist_left = __fsqrts(point_to_line_distance_squared3d(&probe_left,
                &actor->danger_zone.position, &danger_movement));
        unsigned __int8 left_ok = (found_left && dist_left > actor->danger_zone.danger_radius) ? 1 : 0;

        unsigned __int8 found_right = actor_move_try_evasion_vector(actor_index, &perp_right, escape_distance,
                max_ledge_height, &ledge_right, &probe_result);
        float dist_right = __fsqrts(point_to_line_distance_squared3d(&probe_right,
                &actor->danger_zone.position, &danger_movement));
        unsigned __int8 right_ok = (found_right && dist_right > actor->danger_zone.danger_radius) ? 1 : 0;

        if ( found_left )
        {
            if ( !found_right )
            {
                escape_is_ledge = ledge_left;
                escape_direction = 0;
                result = left_ok;
                goto write_out;
            }

            escape_is_ledge = ledge_right;
            float dist_difference = (dist_left - dist_right);
            if ( ledge_left > (unsigned int)ledge_right
              || left_ok > (unsigned int)right_ok
              || dist_difference > 0.30000001f )
            {
                escape_direction = 0;
                escape_is_ledge = ledge_left;
                result = 1;
                goto write_out;
            }
            if ( ledge_right <= (unsigned int)ledge_left
              && right_ok <= (unsigned int)left_ok
              && dist_difference >= -0.30000001f )
            {
                escape_direction = 4;
                escape_is_ledge = ledge_left;
                result = left_ok;
                goto write_out;
            }
            result = 1;
        }
        else
        {
            if ( !found_right )
                goto write_out;
            escape_is_ledge = ledge_right;
            result = right_ok;
        }

        escape_direction = 1;
    }

write_out:
    *escape_direction_reference = escape_direction;
    *escape_distance_reference = escape_distance;
    *escape_is_ledge_reference = escape_is_ledge;
    *alignment_vector_reference = escape_heading;
    return result;
}
