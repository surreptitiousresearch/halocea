/* biped_falling_damage @0x837AE5A0 — apply falling / terminal-velocity damage to a biped after a collision.
 * Below the minimum damage velocity no damage is dealt, unless the biped's downward speed exceeded the global
 * maximum falling velocity (a terminal-velocity impact): then the maximum-distance ("crush") damage effect is
 * applied and, outside a running game engine, an unattended AI biped is deleted outright. Above the minimum
 * damage velocity the falling-damage effect is applied, scaled linearly (clamped 0..1) between the minimum and
 * maximum damage velocities. Bipeds whose tag opts out (definition biped flag 0x80), that are already killed,
 * that are damage-immune, or (unless simulating an update) in a jetpack cheat are spared.
 *
 * Falling-damage globals (float* over game_globals_falling_damage, per biped_falling_danger.c): [35]
 * runtime_maximum_falling_velocity, [36] runtime_minimum_damage_velocity, [37] runtime_maximum_damage_velocity;
 * (int*)[7] = falling_damage effect index, (int*)[14] = maximum_distance_damage effect index. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/game_globals_definition.h"
#include "headers/damage_data.h"
#include "headers/unit_datum.h"
#include "headers/biped_definition.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/unit_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/game_globals_falling_damage.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);
extern uint8_t game_engine_running(void);
extern int player_index_from_unit_index(int unit_index);
extern void object_delete(int object_index);

void biped_falling_damage(int biped_index, float collision_velocity)
{
    game_globals_falling_damage *falling_damage = (game_globals_falling_damage *)global_game_globals->falling_damage.address;
    unit_datum *biped = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);
    biped_definition *definition = TAG_GET(biped_definition, biped->definition_index);

    char immune = 0;
    if ( (biped->unit.flags & (1u << _unit_no_falling_damage_bit)) != 0 || (definition->biped.flags & (1u << _biped_immune_to_falling_damage_bit)) != 0 )
        immune = 1;
    if ( g_running_simulated_update == 1 )
        immune = 1;

    if ( cheat.jetpack && biped->unit.player_index != -1 )
        return;

    if ( collision_velocity <= falling_damage->runtime_minimum_damage_velocity )   /* runtime_minimum_damage_velocity */
    {
        if ( (definition->biped.flags & (1u << _biped_flying_bit)) == 0
            && biped->object.translational_velocity.k < -falling_damage->runtime_maximum_falling_velocity )   /* max falling velocity */
        {
            if ( !immune && (biped->object.damage_flags & (1u << _object_dead_bit)) == 0 )
            {
                damage_data damage;
                damage_data_new(&damage, falling_damage->maximum_distance_damage.index);   /* maximum_distance_damage effect */
                object_cause_damage(&damage, biped_index, -1, -1, -1, nullptr);
            }
            if ( !game_engine_running()
                && (biped->object.flags & (1u << _object_outside_of_map_bit)) != 0
                && player_index_from_unit_index(biped_index) == -1 )
            {
                object_delete(biped_index);
            }
        }
    }
    else if ( !immune )
    {
        damage_data damage;
        damage_data_new(&damage, falling_damage->falling_damage.index);   /* falling_damage effect */
        float scale = (collision_velocity - falling_damage->runtime_minimum_damage_velocity) / (falling_damage->runtime_maximum_damage_velocity - falling_damage->runtime_minimum_damage_velocity);
        damage.scale = scale;
        if ( scale < 0.0f )
            damage.scale = 0.0f;
        else if ( scale > 1.0f )
            damage.scale = 1.0f;
        object_cause_damage(&damage, biped_index, -1, -1, -1, nullptr);
    }
}
