/* unit_new @0x836D3F18 — one-time per-object initialization of a freshly placed unit (biped/vehicle). Fetches
 * the object datum (salted object-header index) and its unit definition (TAG_INSTANCE). Bails returning 0 if the
 * definition has no unit block (dword at +68 == -1). Otherwise resets the whole unit runtime block to its
 * spawn defaults: clears/invalidates the dozens of seat/weapon/grenade/animation/vehicle/AI-dialogue index and
 * state fields (mostly to -1 or 0), seeds five orientation-matrix slots from the object's forward vector,
 * determines the AI dialogue variant, sets the "just spawned" object flags (0x6000), and — for actor-driven
 * spawns — rolls the initial berserk/whatever chance from the definition, defaults the team index outside a
 * running game engine, applies the base seat/weapon label, adds the definition's initial weapons (unless this
 * is a network client), and creates any mounted weapons when the definition declares seats with a mounted
 * weapon. Returns 1 on success.
 *
 * Field offsets are raw byte offsets into the unit datum (which extends the object datum well past its 500-byte
 * head); clean decompiler output (no reg-alloc failure), transcribed faithfully. The object forward vector lives
 * at +116/+120/+124 (object.forward) and is copied into the +548..+616 matrix/vector slots. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include <string.h>
#include "headers/game_connection.h"
#include "headers/unit_animation_overlay_action.h"
#include "headers/base_seat.h"
#include "headers/object_flags.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


#include <stdint.h>
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern uint8_t game_engine_running(void);
extern int16_t game_connection(void);
extern void unit_dialogue_determine_variant(int unit_index);
extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);
extern void unit_add_initial_weapons(int unit_index);
extern void ai_create_mounted_weapons_for_unit(int unit_index);

uint8_t unit_new(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);

    if ( definition->object.animation_graph.index == -1 )
        return 0;

    unit->unit.is_from_network_data_valid = 0;
    unit->unit.last_completed_client_update_id = -1;
    unit->unit.equipment_object_index = -1;
    unit->unit.weapon_object_indices[0] = -1;
    unit->unit.weapon_object_indices[1] = -1;
    unit->unit.weapon_object_indices[2] = -1;
    unit->unit.weapon_object_indices[3] = -1;
    unit->unit.animation.base_seat_index = _base_seat_stand;
    unit->unit.current_weapon_index = -1;
    unit->unit.desired_weapon_index = -1;
    unit->unit.current_grenade_index = -1;
    unit->unit.desired_grenade_index = -1;
    unit->unit.current_zoom_level = -1;
    unit->unit.desired_zoom_level = -1;
    unit->unit.player_index = -1;
    unit->unit.actor_index = -1;
    unit->unit.swarm_actor_index = -1;
    unit->unit.swarm_next_unit_index = -1;
    unit->unit.swarm_prev_unit_index = -1;
    unit->unit.parent_seat_index = -1;
    unit->unit.driver_object_index = -1;
    unit->unit.gunner_object_index = -1;
    unit->unit.animation.flags = 0;
    unit->unit.animation.seat_index = -1;
    unit->unit.animation.weapon_index = -1;
    unit->unit.animation.weapon_type_index = -1;
    unit->unit.animation.state = -1;
    unit->unit.animation.action = 0;
    unit->unit.animation.overlay_action = _unit_overlay_action_none;
    unit->unit.animation.aiming_screen_index = -1;
    unit->unit.animation.looking_screen_index = -1;
    unit->unit.animation.action_animation.index = -1;
    unit->unit.animation.overlay_action_animation.index = -1;
    unit->unit.animation.soft_ping_animation.index = -1;
    unit->unit.animation.last_ping_animation_index = -1;
    unit->unit.animation.emotion_index = -1;
    unit->unit.override_emotion_animation_index = -1;
    unit->unit.magic_seat_index = -1;
    unit->unit.animation.aiming_with_euler_screen = 0;
    unit->unit.animation.aiming_screen_bounds.n[0] = 0;
    unit->unit.animation.aiming_screen_bounds.n[1] = 0;
    unit->unit.animation.aiming_screen_bounds.n[2] = 0;
    unit->unit.animation.aiming_screen_bounds.n[3] = 0;
    unit->unit.animation.looking_with_euler_screen = 0;
    unit->unit.animation.looking_screen_bounds.n[0] = 0;
    unit->unit.animation.looking_screen_bounds.n[1] = 0;
    unit->unit.animation.looking_screen_bounds.n[2] = 0;
    unit->unit.animation.looking_screen_bounds.n[3] = 0;
    unit->unit.persistent_control_timer = 0;
    unit->unit.dialogue_index = -1;

    /* seed five orientation-matrix / cached-vector slots from the object's forward vector */
    float forward_z = unit->object.forward.n[2];
    float forward_x = unit->object.forward.n[0];
    int flags = unit->unit.flags;
    float forward_y = unit->object.forward.n[1];
    unit->unit.looking_vector.n[0] = forward_x;
    unit->unit.looking_vector.n[1] = forward_y;
    unit->unit.looking_vector.n[2] = forward_z;
    unit->unit.desired_looking_vector.n[0] = forward_x;
    unit->unit.desired_looking_vector.n[1] = forward_y;
    unit->unit.desired_looking_vector.n[2] = forward_z;
    unit->unit.aiming_vector.n[0] = forward_x;
    unit->unit.aiming_vector.n[1] = forward_y;
    unit->unit.aiming_vector.n[2] = forward_z;
    unit->unit.desired_aiming_vector.n[0] = forward_x;
    unit->unit.desired_aiming_vector.n[1] = forward_y;
    unit->unit.desired_aiming_vector.n[2] = forward_z;
    unit->unit.desired_facing_vector.n[0] = forward_x;
    unit->unit.desired_facing_vector.n[1] = forward_y;
    unit->unit.desired_facing_vector.n[2] = forward_z;
    unit->unit.flags = flags | (1u << _unit_must_set_up_dialogue_bit);

    memset(&unit->unit.speech, 0, sizeof(unit->unit.speech));
    unit->unit.speech.last_speech_finished_time = -1;
    unit_dialogue_determine_variant(unit_index);
    memset(&unit->unit.attackers[0], -1, 0x40u);
    unit->unit.last_damage_category = 0;
    unit->unit.delayed_damage_timer = 0;
    unit->unit.delayed_damage_attacker_object_index = -1;
    unit->unit.time_of_death = -1;
    unit->unit.fake_encounter_index = -1;
    unit->unit.fake_squad_index = -1;
    unit->unit.delayed_damage_peak = 0.0f;
    unit->unit.flaming_death_delay = 0;
    unit->unit.integrated_light_battery = 1.0f;
    unit->unit.flaming_death_attacker_object_index = -1;
    unit->unit.killing_spree_count = 0;
    unit->unit.killing_spree_last_time = -1;

    __int16 grenade_type = definition->unit.grenade_type;
    if ( (unsigned int)grenade_type <= 1 )
    {
        __int16 grenade_count = definition->unit.grenade_count;
        if ( grenade_count >= 0 )
            unit->unit.grenade_counts[grenade_type] = grenade_count;
    }

    /* just spawned: request both lighting recomputes */
    unit->object.flags |= (1u << _object_dynamic_lighting_recompute_bit)
                        | (1u << _object_static_lighting_recompute_bit);

    /* actor-spawn random roll gated by three positive definition scalars */
    if ( definition->unit.feign_death_threshold > 0.0f
      && definition->unit.feign_death_time > 0.0f
      && definition->unit.feign_death_chance > 0.0f )
    {
        unsigned int *seed = get_global_random_seed_address();
        int current_flags = unit->unit.flags;
        int rolled_flags = current_flags | (1u << _unit_feign_death_allowed_bit);
        if ( real_seed_random(seed) >= (double)definition->unit.feign_death_chance )
            rolled_flags = current_flags & ~(1u << _unit_feign_death_allowed_bit);
        unit->unit.flags = rolled_flags;
    }

    if ( !game_engine_running() && (!unit->object.owner_team_index || unit->object.owner_team_index == -1) )
        unit->object.owner_team_index = definition->unit.default_team;

    unit_set_or_test_seat_and_weapon_label(unit_index, base_seat_labels[_base_seat_stand], nullptr, 1u);

    if ( game_connection() != _game_connection_network_client )
        unit_add_initial_weapons(unit_index);

    /* create mounted weapons if any seat in the definition declares a mounted-weapon object index */
    int seat_count = definition->unit.seats.count;
    unsigned __int8 has_mounted_weapon = 0;
    if ( seat_count > 0 )
    {
        int seat_index = 0;
        while ( ((unit_seat *)definition->unit.seats.address)[seat_index].built_in_actor_reference.index == -1 )
        {
            seat_index = (__int16)(seat_index + 1);
            if ( seat_index >= seat_count )
                goto done_seat_scan;
        }
        has_mounted_weapon = 1;
    }
done_seat_scan:
    if ( has_mounted_weapon )
        ai_create_mounted_weapons_for_unit(unit_index);

    return 1;
}
