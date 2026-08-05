/* ai_communication_get_player_rating @0x837CC9E0 — score how "worth talking to a player" a unit's position is,
 * used to weight AI dialogue. Over all players within 30 world units of the unit's head, computes a rating that
 * grows as the nearest player gets closer (1.0 far .. 2.0 within 3 units, +0.5 with clear line of sight) and is
 * reduced when the player is looking almost straight at the unit. Returns the best rating (1.0 if no players
 * exist at all, 0.0 if players exist but none qualify), and optionally reports the chosen player's unit index
 * and distance.
 *
 * Deviations: Hex-Rays widens single-precision math to double (fpN) — kept as float. The final
 * `return *((float*)&v26+1)` is the usual single/double register-aliasing artifact: the value returned is the
 * computed rating. Object header records are addressed in the 3-dword (3*idx+2) form the decompiler used. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/data_iterator.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"
extern float __fsqrts(float);


extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern int object_get_ultimate_parent(int object_index);
extern uint8_t scenario_test_pvs(int16_t cluster_index0, int16_t cluster_index1);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern void unit_get_aiming_vector(int unit_index, real_vector3d *aiming_vector);

float ai_communication_get_player_rating(int unit_index, uint8_t test_line_of_sight,
                                         int *unit_index_reference, float *distance_reference)
{
    float best_rating = 0.0f;
    float best_distance = 3.4028235e38f;
    int best_unit = -1;
    char any_player = 0;

    real_point3d unit_head;
    unit_get_head_position(unit_index, &unit_head);

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for ( player_datum *player = data_iterator_next(&iterator);
          player;
          player = data_iterator_next(&iterator) )
    {
        int player_unit = player->unit_index;
        if ( player_unit == -1 )
            continue;
        any_player = 1;

        real_point3d player_head;
        unit_get_head_position(player_unit, &player_head);
        float dx = unit_head.n[0] - player_head.n[0];
        float dy = unit_head.n[1] - player_head.n[1];
        float dz = unit_head.n[2] - player_head.n[2];
        float distance_sq = dx * dx + (dz * dz + dy * dy);
        if ( distance_sq >= 900.0f )
            continue;

        char pvs_blocked = 0;
        char clear_los = 0;
        if ( test_line_of_sight )
        {
            int16_t unit_cluster = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_get_ultimate_parent(unit_index))->datum)->object.location.cluster_index;
            int16_t player_cluster = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_get_ultimate_parent(player_unit))->datum)->object.location.cluster_index;
            if ( unit_cluster == 0xFFFF || player_cluster == -1 || scenario_test_pvs(unit_cluster, player_cluster) )
            {
                real_vector3d delta;
                delta.n[0] = unit_head.n[0] - player_head.n[0];
                delta.n[1] = unit_head.n[1] - player_head.n[1];
                delta.n[2] = unit_head.n[2] - player_head.n[2];
                collision_result collision;
                uint8_t hit = collision_test_vector((1u << _collision_test_front_facing_surfaces_bit) | (1u << _collision_test_back_facing_surfaces_bit)
                        | (1u << _collision_test_ignore_two_sided_surfaces_bit) | (1u << _collision_test_structure_bit), &player_head, &delta, -1, &collision);
                if ( distance_sq < 9.0f || !hit )
                    clear_los = 1;
            }
            else
            {
                pvs_blocked = 1;
            }
        }

        if ( pvs_blocked )
            continue;

        float distance = __fsqrts(distance_sq);
        float rating = 1.0f;
        if ( distance < 15.0f )
        {
            if ( distance >= 3.0f )
                rating = (15.0f - distance) * 0.083333336f + 1.0f;
            else
                rating = 2.0f;
            if ( clear_los )
                rating = rating + 0.5f;
            if ( distance > 0.000099999997f )
            {
                real_vector3d aim;
                unit_get_aiming_vector(player_unit, &aim);
                float alignment = (dx * aim.n[0] + (aim.n[2] * dz + aim.n[1] * dy)) / distance;
                if ( alignment > 0.70710677f )
                    rating = rating - (((1.0f - alignment) * 3.4142134f) * 0.34999999f - 0.69999999f);
            }
        }
        if ( rating > best_rating )
        {
            best_unit = player_unit;
            best_rating = rating;
            best_distance = distance;
        }
    }

    if ( !any_player )
        best_rating = 1.0f;
    if ( distance_reference )
        *distance_reference = best_distance;
    if ( unit_index_reference )
        *unit_index_reference = best_unit;
    return best_rating;
}
