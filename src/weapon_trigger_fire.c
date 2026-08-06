/* weapon_trigger_fire @0x836DC300 — fire one shot from a weapon's trigger: the core per-shot handler.
 * It resolves the weapon object datum, its weapon-definition tag, and the trigger definition, then:
 *   - fires the HCEX "player_b30_fire" script event on the b30 map when a player pulls the trigger,
 *   - flags an overload shot for the secondary trigger of overload-type weapons,
 *   - consumes a round from the loaded magazine (unless bottomless-clip is on) and decides whether the
 *     shot actually goes off (did_fire),
 *   - for multi-barrel triggers, cyclically picks the next unused barrel via a seeded RNG and rolls its
 *     firing animation range,
 *   - rolls an "overcharge" chance from the weapon's charge fraction,
 *   - selects the firing/dry effect indices for the chosen barrel and firing state,
 *   - on a real shot: notifies the game engine / AI, stamps the fire time, plays the first-person and
 *     ejection-port animations, adds heat and battery/age, spawns projectiles with the right network
 *     ownership role, applies recoil self-damage to the owner, and possibly detonates on overheat,
 *   - finally advances the trigger's firing-state machine and emits the weapon effect.
 *
 * DEVIATIONS: (1) the barrel-selection inner loop the decompiler emitted as a __PAIR64__ subtraction is
 * a signed (current+1) % barrel_count wrap, rewritten as an explicit cyclic increment. (2) In the
 * client / no-client-side-projectiles branch the decompiler leaves projectile_role holding a stale
 * object-data offset (v67); it is provably dead there (should_create_projectiles == 0 suppresses the
 * call), so it is set to _networked_datum_master with this note. (3) The two `((_cntlzw(x)&0x20)==0)`
 * idioms are just `x != 0`. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/damage_data.h"
#include "headers/networked_datum_role.h"
#include "headers/cheats.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/player_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/weapon_trigger_definition_flags.h"
#include "headers/weapon_trigger_flags.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/trigger_firing_effect.h"
#include "headers/trigger_firing_effect_type.h"
#include "headers/object_flags.h"
#include "headers/weapon_magazine_state.h"
#include "headers/weapon_trigger_state.h"
#include "headers/first_person_weapon_message_type.h"
#include "headers/item_flags.h"
#include "headers/weapon_magazine_flags.h"
#include "headers/weapon_type.h"
#include "headers/weapon_datum_flags.h"
#include "headers/damage_data_flags.h"
#include "headers/weapon_secondary_trigger_mode.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern int weapon_get_owner_object_index(int weapon_index);
extern int player_index_from_unit_index(int unit_index);
extern char *main_get_map_name(void);
extern char *strstr(const char *haystack, const char *needle);
extern float device_group_get_value(int group_index);
extern void hcex_fire_plr_event(const char *event_name, int player_identifier); /* DEVIATION: slot 1 is int, not short -- def src/hcex/hcex_fire_plr_event.cpp + prologue stw r4 @0x823E4730 */
extern uint32_t *get_global_random_seed_address(void);
extern uint16_t seed_random(uint32_t *seed);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern float real_seed_random(uint32_t *seed);
extern uint8_t game_engine_running(void);
extern void game_engine_weapon_fired(int player_index);
extern int game_time_get(void);
extern int16_t game_connection(void);
extern void first_person_weapon_message_from_weapon(int weapon_index, int16_t message_type);
extern void weapon_trigger_start_ejection_port(int weapon_index, int16_t trigger_index, uint8_t chamber);
extern uint8_t weapon_set_state(int weapon_index, int16_t new_state, uint8_t immediate);
extern void trigger_create_projectiles(int weapon_index, int16_t trigger_index, NetworkedDatumRole projectile_datum_role);
extern void ai_handle_unit_effect(int owner_unit_index, int16_t effect_type, int16_t volume);
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);
extern void weapon_detonate(int weapon_index);
extern int weapon_effect_new(int weapon_index, int effect_index, float effect_scale, float effect_error);

void weapon_trigger_fire(int weapon_index, int16_t trigger_index)
{
    unsigned int trigger = trigger_index;

    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *trigger_state = &weapon->weapon.triggers[trigger_index];
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_trigger_definition *trigger_definition =
        &((weapon_trigger_definition *)definition->weapon.triggers.address)[trigger_index];

    int owner_unit_index = weapon_get_owner_object_index(weapon_index);

    char overload_this_shot = 0;
    char did_fire = 0;
    char overcharged = 0;
    float battery_per_shot = trigger_definition->age_generated_per_round;
    int rounds_per_shot = trigger_definition->rounds_per_shot;
    float effect_scale = 0.0f;   /* v16 */
    float effect_error = 0.0f;   /* v14 */
    int firing_effect_index = -1; /* v17 */
    int damage_effect_index = -1; /* v15 */

    /* HCEX b30-map fire event when a player is the shooter. */
    if ( (weapon->item.flags & (1u << _item_belongs_to_player_bit)) != 0 )
    {
        int16_t shooter_local_player;
        if ( player_index_from_unit_index(owner_unit_index) == -1 )
            shooter_local_player = -1;
        else
            shooter_local_player = DATA_ARRAY_ELEMENT(player_data, player_datum,
                    player_index_from_unit_index(owner_unit_index))->local_player_index;

        if ( hcex_2x_rounds_per_shot )
        {
            rounds_per_shot *= 2;
            battery_per_shot = (battery_per_shot * 2.0f);
        }

        if ( shooter_local_player >= 0 )
        {
            const char *map_name = main_get_map_name();
            if ( strstr(map_name, "b30") )
            {
                if ( device_group_get_value(0) > 0.0 )
                    hcex_fire_plr_event("player_b30_fire", shooter_local_player);
            }
        }
    }

    /* Secondary trigger of an overload-type weapon: this shot is an overload burst. */
    if ( trigger == 1 )
    {
        int overload_type = definition->weapon.secondary_trigger_mode;
        if ( overload_type == _weapon_secondary_trigger_loads_alternate_ammunition
          || overload_type == _weapon_secondary_trigger_loads_multiple_primary_ammunition )
            overload_this_shot = 1;
    }

    /* Consume a round from the loaded magazine. */
    unsigned int magazine_index = (uint16_t)trigger_definition->magazine_index;
    if ( magazine_index == 0xFFFF )
    {
        did_fire = 1;
        goto have_fire_decision;
    }

    {
        weapon_magazine *magazine = &weapon->weapon.magazines[(int16_t)magazine_index];
        weapon_magazine_definition *magazine_def = &((weapon_magazine_definition *)
            definition->weapon.magazines.address)[(int16_t)magazine_index];

        if ( !overload_this_shot
          || weapon->weapon.alternate_shots_loaded < definition->weapon.maximum_alternate_shots_loaded )
        {
            int rounds_loaded = magazine->rounds_loaded;
            if ( (rounds_loaded >= rounds_per_shot || (trigger_definition->flags & (1u << _weapon_trigger_can_fire_with_partial_ammunition_bit)) != 0)
              && ((definition->weapon.flags & (1u << _weapon_cannot_fire_at_maximum_age_bit)) == 0 || weapon->weapon.age < 1.0)
              && (rounds_loaded >= trigger_definition->minimum_rounds_loaded_per_shot
                  || (trigger_state->flags & (1u << _weapon_trigger_released_since_last_shot_bit)) == 0) )
            {
                int remaining = rounds_loaded - rounds_per_shot;
                if ( cheat.bottomless_clip || (magazine->rounds_loaded = remaining, remaining > 0) )
                {
                    /* magazine emptied but marked "reload from reserve" — kick off a reload. */
                    if ( (magazine_def->flags & (1u << _weapon_magazine_must_be_chambered_every_shot_bit)) != 0 )
                    {
                        magazine->state = _weapon_magazine_reloaded;
                        magazine->state_timer = 0;
                    }
                }
                else
                {
                    magazine->rounds_loaded = 0;
                }
                did_fire = 1;
            }
        }
    }

have_fire_decision:
    if ( cheat.bottomless_clip )
        did_fire = 1;

    int barrel_count = trigger_definition->firing_effects.count;
    int16_t barrel_rounds; /* v29 — rounds left before advancing barrel / rolled animation range */
    if ( barrel_count > 0 )
    {
        barrel_rounds = trigger_state->firing_effect_shots_remaining;
        if ( barrel_rounds <= 0 )
        {
            int16_t start_barrel = trigger_state->firing_effect_index;
            int current_barrel;
            if ( (trigger_definition->flags & (1u << _weapon_trigger_random_firing_effects_bit)) != 0 )
            {
                unsigned int *seed = get_global_random_seed_address();
                current_barrel = (int)seed_random(seed) % trigger_definition->firing_effects.count;
            }
            else
            {
                current_barrel = trigger_state->firing_effect_index;
            }

            do
            {
                /* Reset the used-barrel bitmask once every barrel has fired. */
                if ( trigger_state->firing_effects_used_flags
                        == (1 << trigger_definition->firing_effects.count) - 1 )
                    trigger_state->firing_effects_used_flags = 0;

                int count = trigger_definition->firing_effects.count;
                int used_mask = trigger_state->firing_effects_used_flags;
                int barrel_bit;
                do
                {
                    /* signed (current+1) % count → cyclic advance to the next barrel */
                    int next = current_barrel + 1;
                    current_barrel = (next >= count) ? 0 : next;
                    barrel_bit = 1 << current_barrel;
                }
                while ( (barrel_bit & used_mask) != 0 );

                trigger_firing_effect *barrels =
                    (trigger_firing_effect *)trigger_definition->firing_effects.address;
                trigger_state->firing_effect_index = current_barrel;
                trigger_state->firing_effects_used_flags = barrel_bit | used_mask;

                trigger_firing_effect *barrel = &barrels[(int16_t)current_barrel];
                unsigned int *seed = get_global_random_seed_address();
                barrel_rounds = seed_random_range(seed, barrel->shots_lower_bound,
                                                  barrel->shots_upper_bound);
                trigger_state->firing_effect_shots_remaining = barrel_rounds;
            }
            while ( barrel_rounds <= 0 && current_barrel != start_barrel );
        }

        int16_t active_barrel = trigger_state->firing_effect_index;
        trigger_state->firing_effect_shots_remaining = barrel_rounds - 1;

        /* Overcharge roll: fraction of the weapon's charge past the misfire threshold. */
        float overcharge_threshold = definition->weapon.age_misfire_start;
        trigger_firing_effect *barrel_effects = &((trigger_firing_effect *)
            trigger_definition->firing_effects.address)[active_barrel];
        if ( overcharge_threshold > 0.0 && overcharge_threshold < 1.0
          && weapon->weapon.age > overcharge_threshold )
        {
            float overcharge_chance = (((weapon->weapon.age
                            - definition->weapon.age_misfire_start) * definition->weapon.age_misfire_chance)
                    / (1.0f - definition->weapon.age_misfire_start));
            if ( trigger_state->state == _weapon_trigger_releasing )
                overcharge_chance = (overcharge_chance * 2.0f);
            unsigned int *seed = get_global_random_seed_address();
            if ( real_seed_random(seed) < overcharge_chance )
                overcharged = 1;
        }

        int firing_state; /* v49: effect-slot selector (trigger_firing_effect_type) */
        if ( did_fire )
        {
            effect_scale = trigger_state->rate_of_fire;  /* held-charge fraction */
            if ( overcharged )
            {
                firing_state = _trigger_overheated_effect;
                effect_error = 0.0f;
            }
            else
            {
                firing_state = _trigger_firing_effect;
                if ( definition->weapon.heat_overheated_threshold == 0.0 )
                    effect_error = 0.0f;
                else
                    effect_error = (weapon->weapon.heat
                                           / definition->weapon.heat_overheated_threshold);
            }
        }
        else
        {
            firing_state = _trigger_empty_effect;
            effect_scale = 1.0f;
            effect_error = 0.0f;
        }

        firing_effect_index = barrel_effects->effects[firing_state].index;
        damage_effect_index = barrel_effects->damage_effects[firing_state].index;
    }

    if ( did_fire )
    {
        if ( (weapon->item.flags & (1u << _item_belongs_to_player_bit)) != 0 )
        {
            if ( game_engine_running() )
            {
                int player_index = player_index_from_unit_index(owner_unit_index);
                if ( player_index != -1 )
                    game_engine_weapon_fired(player_index);
            }
        }

        weapon->weapon.game_time_last_fired = game_time_get();

        int16_t message_index;
        if ( overcharged )
            message_index = (trigger != 0) ? _first_person_weapon_message_secondary_misfire
                                           : _first_person_weapon_message_primary_misfire;
        else
            message_index = (trigger != 0) ? _first_person_weapon_message_secondary_fire
                                           : _first_person_weapon_message_primary_fire;
        first_person_weapon_message_from_weapon(weapon_index, message_index);
        weapon_trigger_start_ejection_port(weapon_index, trigger_index, 0);

        if ( trigger_definition->illumination_recovery_time > 0.0 )
            trigger_state->illumination = 1.0f;

        if ( !cheat.bottomless_clip )
            weapon->weapon.heat = trigger_definition->heat_generated_per_round + weapon->weapon.heat;

        int in_game = weapon->item.flags & (1u << _item_belongs_to_player_bit);
        if ( in_game || weapon->weapon.heat <= (double)definition->weapon.heat_overheated_threshold )
        {
            if ( weapon->weapon.heat > 1.0 )
                weapon->weapon.heat = 1.0f;
        }
        else
        {
            weapon->weapon.heat = definition->weapon.heat_overheated_threshold;
        }

        if ( in_game )
        {
            if ( !cheat.infinite_ammo )
            {
                float new_battery = (weapon->weapon.age + battery_per_shot);
                weapon->weapon.age = weapon->weapon.age + battery_per_shot;
                if ( new_battery > 1.0 )
                {
                    weapon->weapon.age = 1.0f;
                    weapon->object.flags |= (1u << _object_force_baseline_update_bit); /* object flag: battery depleted */
                }
            }
        }

        weapon_set_state(weapon_index, (trigger != 0) + 1, 0);

        if ( !overcharged )
        {
            if ( overload_this_shot )
            {
                ++weapon->weapon.alternate_shots_loaded;
            }
            else
            {
                char should_create_projectiles;
                NetworkedDatumRole projectile_role;
                int16_t connection = game_connection();
                if ( connection == 1 )
                {
                    if ( (trigger_definition->flags & (1u << _weapon_trigger_client_side_only_bit)) != 0 && allow_client_side_weapon_projectiles == 1 )
                    {
                        should_create_projectiles = 1;
                        projectile_role = _networked_datum_autonomous;
                    }
                    else
                    {
                        should_create_projectiles = 0;
                        projectile_role = _networked_datum_master; /* unused: not created on this client */
                    }
                }
                else
                {
                    should_create_projectiles = 1;
                    if ( connection == 2 )
                    {
                        if ( (trigger_definition->flags & (1u << _weapon_trigger_client_side_only_bit)) != 0
                          && allow_client_side_weapon_projectiles == 1 )
                            projectile_role = _networked_datum_autonomous;
                        else
                            projectile_role = _networked_datum_master;
                    }
                    else
                    {
                        projectile_role = _networked_datum_autonomous;
                    }
                }

                if ( should_create_projectiles == 1 )
                    trigger_create_projectiles(weapon_index, trigger_index, projectile_role);
                ai_handle_unit_effect(owner_unit_index, 1, trigger_definition->firing_noise);
            }
        }

        /* Recoil self-damage on the owner unit. */
        if ( owner_unit_index != -1 && damage_effect_index != -1 )
        {
            unit_datum *owner = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                   owner_unit_index)->datum);
            damage_data recoil_damage;
            damage_data_new(&recoil_damage, damage_effect_index);
            recoil_damage.flags |= (1u << _damage_from_weapon_bit);
            recoil_damage.direction.n[0] = -owner->unit.aiming_vector.n[0];
            recoil_damage.direction.n[1] = -owner->unit.aiming_vector.n[1];
            recoil_damage.direction.n[2] = -owner->unit.aiming_vector.n[2];
            float origin_y = owner->object.bounding_sphere_center.n[1];
            float origin_z = owner->object.bounding_sphere_center.n[2];
            recoil_damage.origin.n[0] = owner->object.bounding_sphere_center.n[0];
            recoil_damage.epicenter.n[0] = recoil_damage.origin.n[0];
            recoil_damage.epicenter.n[1] = origin_y;
            recoil_damage.epicenter.n[2] = origin_z;
            recoil_damage.origin.n[1] = origin_y;
            recoil_damage.origin.n[2] = origin_z;
            object_cause_damage(&recoil_damage, owner_unit_index, -1, -1, -1, nullptr);
        }

        if ( definition->weapon.weapon_type == _weapon_type_plasma_pistol && trigger == 1 )
            weapon->weapon.flags |= (1u << _weapon_overheat_recoil_bit);
    }

    /* Overheat detonation. */
    if ( weapon->weapon.heat > (double)definition->weapon.heat_detonation_threshold )
    {
        unsigned int *seed = get_global_random_seed_address();
        if ( real_seed_random(seed) < (double)definition->weapon.overheated_explosion_fraction )
            weapon_detonate(weapon_index);
    }

    /* Advance the trigger's firing-state machine. */
    char next_state; /* v62 */
    if ( !did_fire )
    {
        next_state = _weapon_trigger_firing_held;
        trigger_state->state_timer = -1;
    }
    else if ( trigger_state->state != _weapon_trigger_releasing || overcharged )
    {
        if ( (trigger_definition->flags & (1u << _weapon_trigger_tracks_projectile_bit)) != 0 )
        {
            next_state = _weapon_trigger_tracking;
            trigger_state->state_timer = -1;
        }
        else
        {
            next_state = _weapon_trigger_idle;
            trigger_state->idle_ticks = 0;
            trigger_state->state_timer = 0;
        }
    }
    else
    {
        goto emit_effect;
    }

    trigger_state->state = next_state;

emit_effect:
    trigger_state->flags &= ~(1u << _weapon_trigger_released_since_last_shot_bit);
    weapon_effect_new(weapon_index, firing_effect_index, effect_scale, effect_error);
}
