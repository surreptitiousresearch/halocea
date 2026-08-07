/* actor_new @0x8371CDB8 — allocate a fresh actor datum for an actor-variant definition and initialise its entire
 * field block to default (mostly -1 / 0) values. Resolves the variant's referenced actor (character) definition,
 * seeds the type/flags, rolls the optional "leader" chance, points the facing/aim vectors at global_forward3d,
 * then runs actor_type_initialize. Returns the new actor index, or -1 on bad variant / allocation failure.
 *
 * Faithful to the decompiler's raw datum-offset writes (the actor_datum struct is only partially mapped). */

#include <stdint.h>
#include <string.h>
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/actor_action.h"
#include "headers/actor_fire_state.h"
#include "headers/actor_target_type.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/destination_type.h"
#include "headers/primary_look_priority.h"
#include "headers/secondary_look_type.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern void actor_clear_discarded_firing_positions(uint16_t actor_index, uint8_t clear_temporary_only);
extern void actor_type_initialize(uint16_t actor_index);

int actor_new(int variant_definition_index)
{
    if ( variant_definition_index == -1 )
        return -1;

    actor_variant_definition *variant = TAG_GET(actor_variant_definition, variant_definition_index);
    int character_definition_index = variant->actor_reference.index;  /* variant+0x10 */
    if ( character_definition_index == -1 )
        return -1;

    actor_definition *character = TAG_GET(actor_definition, character_definition_index);
    int result = datum_new(actor_data);
    if ( result == -1 )
        return result;

    unsigned int character_flags = character->flags;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, result);
    actor->meta.definition_index = character_definition_index;
    actor->meta.variant_definition_index = variant_definition_index;
    actor->meta.swarm = (character_flags & (1u << _actor_definition_swarm_actor_bit)) != 0;
    int16_t actor_type = character->type;
    actor->meta.unit_index = -1;
    actor->meta.disconnected_squad_index = -1;
    actor->meta.type = actor_type;
    actor->meta.unique_leader = 0;
    actor->meta.encounter_index = -1;
    actor->meta.platoon_index = -1;
    actor->meta.encounterless = 0;
    actor->meta.disconnected_encounter_index = -1;
    actor->meta.disconnected_squad_index = -1;
    actor->meta.swarm_unit_count = 0;
    actor->meta.swarm_original_unit_count = 0;
    actor->meta.swarm_unit_index = -1;
    actor->meta.swarm_cache_index = -1;
    actor->meta.frozen = 1;
    actor->meta.active = 0;
    actor->meta.last_active_time = -1;
    actor->meta.dormant = 1;
    actor->meta.dormant_desire = 1;
    actor->meta.service_timer = 0;
    actor->meta.first_prop_index = -1;
    actor->meta.interesting_orphan_index = -1;
    actor->firing_positions.current_position_index = -1;
    actor->state.initial_state = -1;
    actor->state.default_state = -1;
    actor->state.last_default_state_time = -1;
    actor->state.command_list_immediate = 0;
    actor->state.command_list_index = -1;
    actor->state.last_command_list_time = -1;
    actor->state.action = actor_action_none;
    actor->state.mode = _actor_mode_alert;
    actor->state.combat_status = _actor_combat_status_none;
    actor->state.artificial_combat_status = _actor_combat_status_none;
    actor->state.suspicion_combat_status = _actor_combat_status_none;
    actor->state.uncertain_combat_timer = -1;
    actor->state.searching = 0;
    actor->state.flying = (character->flags & (1u << _actor_definition_flying_bit)) != 0;
    actor->input.pathfinding_surface_index = -1;
    actor->input.vehicle_index = -1;
    actor->external_orders.defending = 0;
    actor->external_orders.pursuit_is_coordinator = 0;
    actor->external_orders.pursuit_group_prop_index = -1;
    actor->external_orders.desired_target_type = 0;
    actor->external_orders.conversation_index = -1;
    memset(&actor->emotions, 0, 0x68u);
    actor->emotions.last_active_cover_seeking_time = -1;
    actor->emotions.last_melee_check_time = -1;
    actor->emotions.original_body_vitality = 1.0f;
    actor->emotions.last_melee_attack_time = -1;
    actor->emotions.last_defensive_cover_seeking_time = -1;
    actor->emotions.last_vehicle_check_time = -1;
    actor->emotions.last_vehicle_charge_time = -1;
    actor->emotions.last_flee_failed_time = -1;
    actor->emotions.corpse_ignore_time = -1;
    actor->emotions.unopposable_friend_ignore_time = -1;
    actor->emotions.unopposable_retreat_prop_index = -1;
    actor->emotions.unopposable_retreat_start_time = -1;
    actor->emotions.vehicle_ignore_index = -1;
    actor->emotions.vehicle_ignore_time = -1;
    actor->emotions.flee_with_friends_disable_time = -1;
    /* character+144 = actor_definition.moving.glass_ignorance_chance (0x80 moving + 0x10); an earlier
     * pass mislabeled this "leader chance" — it is the spawn-time roll for ignoring breakable surfaces. */
    if ( character->moving.glass_ignorance_chance > 0.0f )
    {
        unsigned int *seed = get_global_random_seed_address();
        actor->emotions.ignorant_of_broken_surfaces = real_seed_random(seed) < character->moving.glass_ignorance_chance;
    }
    actor->orders.look.primary_priority = _primary_priority_none;
    actor->orders.move.destination.destination_type = _destination_none;
    actor->control.path.destination_orders.destination_type = _destination_none;
    actor->control.path.destination_orders.ignore_target_object_index = -1;
    actor->control.path.destination.surface_index = -1;
    memset(&actor->control.path.path.valid, 0, 0x5Cu);
    actor->control.moving = 0;
    actor->control.moving_forced_by_aiming = 0;
    actor->control.fire_state_timer = 0;
    actor->control.burst_disable_timer = 0;
    actor->control.trigger_delay_timer = 0;
    actor->control.blocked_communication_timer = 0;
    actor->control.current_fire_target_timer = 0;
    actor->control.fire_state = actor_fire_state_holding;
    actor->control.___u58.current_fire_target_prop_index = -1;              /* recovered: *(int*)&control._unresolved_type_1A4[0] -> current_fire_target union int member */
    actor->control.grenade_last_check_time = -1;
    actor->control.grenade_current_prop_index = -1;
    /* recovered: *((_DWORD*)actor + 370..373) -> control.vector_avoidance_clear_times[8][2] (0x15C, 16 bytes) set to 0xFF */
    memset(actor->control.vector_avoidance_clear_times, -1, sizeof(actor->control.vector_avoidance_clear_times));
    actor->control.secondary_look_type = _secondary_look_none;
    actor->control.secondary_look_timer = 0;
    actor->control.vector_avoidance_current_direction = -1;
    actor->control.vector_avoidance_sharp_turn_timer = -1;
    actor->control.desired_aiming_vector = *global_forward3d;
    actor->control.desired_facing_vector = *global_forward3d;
    actor->control.desired_looking_vector.n[0] = global_forward3d->n[0];
    actor->control.desired_looking_vector.n[1] = global_forward3d->n[1];
    actor->control.idle_vocalization_combat = 0;
    actor->target.target_type = actor_target_none;
    actor->control.desired_looking_vector.n[2] = global_forward3d->n[2];
    actor->control.idle_vocalization_timer = 30;
    actor->target.target_prop_index = -1;
    actor->target.target_last_visible_time = -1;
    actor->target.since_any_target_visible_timer = -1;
    actor_clear_discarded_firing_positions(result, 0);
    actor->firing_positions.pursuit_prop_index = -1;
    actor_type_initialize(result);
    return result;
}
