/* unit_damage_aftermath @0x836D5940 — the unit-specific tail of object_cause_damage's per-target loop,
 * run after the generic object_damage_aftermath has applied scoring/acceleration. Handles everything that
 * is special about a *unit* (biped/vehicle) taking damage: bleeding/burning bookkeeping, feign-death,
 * the damage "ping" reaction animation, AI death/damage reactions, the camera/network unzoom, controller
 * stun, and — on a killing blow — unit death plus its network replication.
 *
 * ABI (8 args, verified from prologue 0x836D5940 and call site 0x836B2160):
 *   r3 unit_index, r4 damage_data, r5 being_damaged_flags,
 *   f1 shield_damage, f2 body_damage, f3 body_damage_multiplier  (single-precision floats),
 *   r9 body_part, r10 unit_is_alive_gate.
 * The decompiler invents three trailing integer params (a9/a10/a11) out of the float args' reserved GPR
 * slots; a10 is really r9 (body_part) and a11 is really r10 (the gate). body_damage_multiplier is unused
 * by the body but occupies f3.
 *
 * Typed access via unit_datum / damage_definition / unit_definition / game_globals_player_information
 * (all offsets DB-verified). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_definition_flags.h"
#include "headers/damage_definition.h"
#include "headers/damage_effect_definition.h"
#include "headers/damage_definition_flags.h"
#include "headers/game_globals_player_information.h"
#include "headers/game_globals_tag.h"
#include "headers/damage_data.h"
#include "headers/damage_data_flags.h"
#include "headers/object_being_damaged_flags.h"
#include "headers/real_vector2d.h"
#include "headers/real_vector3d.h"
#include "headers/unit_kill_network_data.h"
#include "headers/player_datum.h"
#include "headers/networked_datum_role.h"
#include "headers/object_damage_flags.h"
#include "headers/object_type.h"
#include "headers/unit_flags.h"


#include "headers/real_vector3d.h"
typedef struct _field_properties_definition _field_properties_definition;
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern uint8_t game_engine_running(void);
extern void *datum_try_and_get(const data_array *data, int index);
extern data_array *player_data;
extern float normalize2d(real_vector2d *v);
extern float signed_angle_between_vectors2d(const real_vector2d *a, const real_vector2d *b);

extern uint8_t unit_try_and_exit_seat(int object_index, uint8_t should_allow_clients);
extern void unit_start_flaming_to_death(int unit_index, int attacker_object_index);
extern void unit_ping_animation(int unit_index, uint8_t died, uint8_t feign_death, uint8_t died_flying, uint8_t ignore_hard_pings, uint8_t force_hard_pings, float angle, int16_t body_part, const real_vector2d *alignment_vector, uint8_t gate);
extern void game_engine_player_damaged_player(int killing_player_index, int dead_player_index, uint8_t friendly_fire);
extern void unit_record_damage(int unit_index, float damage, int16_t damage_category, uint8_t died, int owner_player_index, int16_t owner_team_index, int owner_object_index);
extern int unit_make_damage_sound(int unit_index, damage_data *damage_data, uint8_t died, uint8_t died_instantly, float body_damage, float shield_damage);
extern void unit_unzoom(int unit_index);
extern void ai_handle_death(int unit_index, int owner_object_index, int16_t damage_category);
extern void ai_handle_damage(int unit_index, int shooter_object_index, int16_t damage_category, float fraction, real_vector3d *damage_velocity, uint8_t delayed);
extern void unit_died(int unit_index, uint8_t feigned);
extern void unit_kill_to_network(unit_kill_network_data data);  /* 32-byte struct passed by value (r3..r6) */
extern int field_translated_index_unregister_local_index(const _field_properties_definition *field_properties_definition, int local_index);
#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"

void unit_damage_aftermath(int unit_index, damage_data *damage_data, char being_damaged_flags,
                           float shield_damage, float body_damage, float body_damage_multiplier,
                           int16_t body_part, uint8_t unit_is_alive_gate)
{
    float total_damage = shield_damage + body_damage;

    object_header_datum *header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index);
    unit_datum *unit = (unit_datum *)header->datum;

    /* damage effect definition (jpt!): its damage block (+452 = damage_effect_definition.damage) */
    damage_definition *damage =
        &TAG_GET(damage_effect_definition, damage_data->definition_index)->damage;
    /* the unit's own tag definition */
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);

    uint8_t lethal_flag = being_damaged_flags & (1u << _object_being_damaged_body_depleted_bit);
    uint8_t feign_death = 0;
    uint8_t should_cause_ping = 0;
    uint8_t died = lethal_flag;

    /* bleeding/burning: record the ongoing damage source on the unit */
    if ( unit_is_alive_gate == 1 )
    {
        float accumulated = unit->object.recent_body_damage + unit->object.recent_shield_damage;
        if ( accumulated > 0.0f )
        {
            int16_t damage_category = damage->category;
            float current = unit->unit.delayed_damage_peak;
            unit->unit.delayed_damage_timer = 45;
            unit->unit.last_damage_category = damage_category;
            if ( current > accumulated )
                accumulated = current;
            unit->unit.delayed_damage_peak = accumulated;
            if ( damage_data->owner_object_index != -1 )
                unit->unit.delayed_damage_attacker_object_index = damage_data->owner_object_index;
        }
    }

    unsigned int unit_flags = unit->unit.flags;
    if ( (unit_flags & (1u << _unit_active_camouflaged_bit)) != 0 )  /* drain active camouflage by the definition's amount */
    {
        float drained = unit->unit.active_camouflage - damage->active_camouflage_damage;
        unit->unit.active_camouflage = drained;
        if ( drained < 0.0f )
            unit->unit.active_camouflage = 0.0f;
    }

    /* should_cause_ping and feign-death evaluation */
    if ( unit_is_alive_gate == 1 )
    {
        should_cause_ping = (lethal_flag && damage->instantaneous_acceleration >= 2.0f) ? 1 : 0;

        if ( !lethal_flag && (unit_flags & (1u << _unit_feign_death_allowed_bit)) != 0 )
        {
            float feign_threshold = definition->unit.feign_death_threshold;
            if ( feign_threshold > 0.0f && definition->unit.feign_death_time > 0.0f
              && unit->object.body_vitality > 0.0f && unit->object.recent_body_damage > feign_threshold )
            {
                float feign_ticks = (definition->unit.feign_death_time
                                     + real_seed_random_range(get_global_random_seed_address(), 0.0, 1.0))
                                    * 30.0f;
                feign_death = 1;
                unit->object.damage_flags |= (1u << _object_dead_bit);  /* body-depleted flag */
                if ( feign_ticks < 1.0f )
                    feign_ticks = 1.0f;
                unit->unit.feign_death_timer = (int)feign_ticks;
            }
        }
    }

    real_vector2d alignment_vector = { { 0.0f, 0.0f } };
    float ping_angle = 0.0f;
    uint8_t alignment_valid = 0;
    uint8_t ignore_hard_pings = 0;
    uint8_t force_hard_pings = 0;
    uint8_t died_flying = 0;

    if ( unit->unit.player_index == -1
      && lethal_flag
      && (damage->flags & (1u << _damage_causes_flaming_death_bit)) != 0
      && (definition->unit.flags & (1u << _unit_runs_around_flaming)) != 0
      && (unit->object.parent_object_index == -1 || unit_try_and_exit_seat(unit_index, 0)) )
    {
        /* unmanned unit forced out of its seat / sent to a flaming death; no ping reaction */
        if ( unit_is_alive_gate == 1 )
            unit_start_flaming_to_death(unit_index, damage_data->owner_object_index);
        died = 0;
    }
    else if ( (damage_data->flags & (1u << _damage_silent_bit)) != 0
           || (!lethal_flag && !feign_death && (unit->object.damage_flags & (1u << _object_dead_bit)) != 0)
           || (unit->unit.flags & (1u << _unit_impervious_bit)) != 0
           || (damage->flags & (1u << _damage_does_not_ping_units_bit)) != 0 )
    {
        /* suppressed: silent/already-dead/no-ping damage — leave the network struct's died_flying clear */
        died_flying = 0;
    }
    else
    {
        /* play the damage ping reaction, oriented by the angle between the damage direction and facing */
        real_vector2d damage_direction = *(real_vector2d *)damage_data->direction.n;
        real_vector2d facing = *(real_vector2d *)unit->object.forward.n;
        if ( normalize2d(&damage_direction) > 0.0f && normalize2d(&facing) > 0.0f )
        {
            ping_angle = signed_angle_between_vectors2d(&damage_direction, &facing);
            alignment_valid = 1;
        }

        if ( (definition->unit.flags & (1u << _unit_definition_resists_pings_bit)) != 0 && (damage->flags & (1u << _damage_pings_resistant_units_bit)) == 0 )
            ignore_hard_pings = 1;
        if ( unit->unit.flaming_death_delay )
            ignore_hard_pings = 1;
        if ( (being_damaged_flags & (1u << _object_being_damaged_region_destroyed_bit)) != 0
          || (being_damaged_flags & (1u << _object_being_damaged_shield_depleted_bit)) != 0
          || being_damaged_flags < 0 )   /* sign bit = _object_being_damaged_force_hard_ping_bit (7) */
            force_hard_pings = 1;

        died_flying = 1;
        if ( alignment_valid )
            alignment_vector = damage_direction;

        /* attested 10-param: phantom r9-shadow arg dropped; angle precedes body_part (f1 shadows r9) */
        unit_ping_animation(unit_index, lethal_flag, feign_death, died_flying, ignore_hard_pings,
                            force_hard_pings, ping_angle, body_part,
                            alignment_valid ? &alignment_vector : 0, unit_is_alive_gate);
    }

    /* Restored (dropped-call scan): the binary latches the victim player's respawn timer into the
     * kill-replication payload HERE (0x836D5CF8-0x836D5D20, on the common path before unit_died,
     * which can restart the timer) — the reconstruction had dropped this block, leaving its
     * datum_try_and_get extern dangling and network_data.respawn_timer stuck at the {0} default. */
    int network_respawn_timer = 0;
    if ( unit->unit.player_index != -1 )
    {
        player_datum *victim_player = datum_try_and_get(player_data, unit->unit.player_index);
        if ( victim_player )
            network_respawn_timer = victim_player->respawn_timer;
    }

    /* player-damage events, damage record, and damage sound */
    if ( unit_is_alive_gate == 1 )
    {
        int attacker_player_index = damage_data->owner_player_index;
        int victim_player_index = unit->unit.player_index;
        if ( attacker_player_index != -1 && victim_player_index != -1 )
            game_engine_player_damaged_player(attacker_player_index, victim_player_index,
                                              (being_damaged_flags & (1u << _object_being_damaged_by_friendly_bit)) != 0);

        if ( damage_data->owner_player_index != -1 || damage_data->owner_object_index != -1 )
            unit_record_damage(unit_index, total_damage, damage->category,
                               died, damage_data->owner_player_index, damage_data->owner_team_index,
                               damage_data->owner_object_index);

        if ( (damage_data->flags & (1u << _damage_silent_bit)) == 0 && (lethal_flag || body_damage > 0.0f || shield_damage > 0.0f) )
            unit_make_damage_sound(unit_index, damage_data, feign_death | died,
                                   (being_damaged_flags & (1u << _object_being_damaged_killed_instantly_bit)) != 0, body_damage, shield_damage);
    }

    if ( body_damage > 0.0f || shield_damage > 0.0f )
        unit_unzoom(unit_index);

    /* AI reactions (bipeds only) */
    if ( unit_is_alive_gate == 1 && unit->object.type == object_type_biped )
    {
        if ( died )
            ai_handle_death(unit_index, damage_data->owner_object_index,
                            damage->category);
        else if ( (unit->object.damage_flags & (1u << _object_dead_bit)) == 0 )
            ai_handle_damage(unit_index, damage_data->owner_object_index,
                             damage->category, total_damage,
                             &damage_data->direction, 0);
    }

    /* controller stun (rumble): grows toward the definition's maximum */
    if ( unit->unit.player_index != -1 && damage->stun > 0.0f
      && (game_engine_running() || stun_enable) )
    {
        float stun = damage->stun * damage_data->scale;
        float max_stun = damage->maximum_stun * damage_data->scale;
        game_globals_player_information *player_information =
            (game_globals_player_information *)global_game_globals->player_information.address;
        if ( stun < 0.0f )
            stun = 0.0f;
        if ( max_stun < 0.0f )
            max_stun = 0.0f;
        else if ( max_stun >= 1.0f )
            max_stun = 1.0f;
        if ( unit->unit.body_stun < max_stun )
        {
            float new_stun = stun + unit->unit.body_stun;
            unit->unit.body_stun = new_stun;
            if ( new_stun > max_stun )
                unit->unit.body_stun = max_stun;
        }

        int16_t max_duration = (int)(player_information->maximum_stun_time * 30.0f);
        int16_t add_duration = (int)(damage->stun_time * 30.0f);
        int16_t min_duration = (int)(player_information->minimum_stun_time * 30.0f);
        if ( unit->unit.body_stun_ticks < min_duration )
            unit->unit.body_stun_ticks = min_duration;
        int16_t duration = add_duration + unit->unit.body_stun_ticks;
        unit->unit.body_stun_ticks = duration;
        if ( duration > max_duration )
            unit->unit.body_stun_ticks = max_duration;
    }

    /* death + network replication */
    if ( unit_is_alive_gate == 1 && (died || feign_death) )
    {
        unit_died(unit_index, feign_death);
        if ( !unit->object.datum_role && died == 1 )
        {
            unit_kill_network_data network_data = { 0 };
            network_data.unit_index = unit_index;
            network_data.should_cause_ping = should_cause_ping;
            network_data.died = lethal_flag;
            network_data.feign_death = feign_death;
            network_data.died_flying = died_flying;
            network_data.ignore_hard_pings = ignore_hard_pings;
            network_data.force_hard_pings = force_hard_pings;
            network_data.should_alignment_vector_be_null = alignment_valid ? 0 : 1;
            network_data.damage_part = body_part;
            network_data.angle = ping_angle;
            network_data.respawn_timer = network_respawn_timer;  /* latched pre-unit_died (see restore note above) */
            if ( alignment_valid )
                network_data.alignment_vector = alignment_vector;
            unit_kill_to_network(network_data);

            if ( (header->flags & (1u << _object_header_being_deleted_bit)) == 0 )
                field_translated_index_unregister_local_index(&field_properties_object_index_definition,
                                                              unit_index);
            unit->object.datum_role = _networked_datum_autonomous;  /* mark death replicated */
        }
    }
}
