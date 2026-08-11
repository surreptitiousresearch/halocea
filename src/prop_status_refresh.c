/* prop_status_refresh @0x837DA400 — the heavy per-tick perception update for one prop: recompute quantized
 * speed/closing-speed/distance/facing, line-of-sight, visibility, audibility and combined awareness; track
 * the represented unit/actor (combat state, swarm membership), assess vehicle/suicide danger, mark
 * unreachable, optionally re-evaluate whether the prop is still desired, and finally recompute the prop's
 * unopposable flag, target weight and look interest.
 *
 * 2026-07-13 full type recovery. The prior raw-offset transcription had systematically mislabeled the
 * death-tracking chain as visibility flags: prop bytes 295/296/297 are dead/really_dead/just_killed
 * (prop_datum ground truth), sourced from object.damage_flags bit 2 — and "really dead" is dead AND
 * unit.feign_death_timer == 0, i.e. a feigning-death unit is dead but not *really* dead.
 *
 * Deviations are marked inline. The two actor_visibility_at_point arguments once flagged as unresolvable
 * Hex-Rays register leftovers were re-derived from disasm: the first call's lighting is
 * `prop->flashlight ? 2 : prop->lighting` (0x837DAC0C-24, same select as the corpse-path call), and the
 * third call's use_frustum is a real computed boolean (0x837DAD00-90: vehicle-driver/mounted-weapon
 * suppress it; for enemies it drops in the becoming/acknowledged band via a subfc/subfe <=3 mask;
 * for non-enemies it requires pre-combat mode AND a dead-or-fighting prop). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/encounter_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/biped_definition.h"
#include "headers/actor_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_position_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/actor_target_type.h"
#include "headers/ai_sound_volume.h"
#include "headers/ai_unit_effect.h"
#include "headers/unit_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_mode.h"
#include "headers/actor_type.h"                /* _actor_type_mounted_weapon */
#include "headers/actor_vehicle_driver_type.h" /* _actor_vehicle_driver_directional_flying */
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/desired_target_type.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/actor_definition_flags.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"
#include "headers/real_point3d.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
extern float __fsqrts(float);
extern int _cntlzw(unsigned int);

extern int game_time_get(void);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern void unit_get_aiming_vector(int unit_index, real_vector3d *aiming_vector);
extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_secondary(int actor_index, int16_t type, int16_t priority, direction_specification *direction);
extern int16_t ai_test_line_of_sight(const real_point3d *p0, int16_t p0_cluster_index, const real_point3d *p1, int16_t p1_cluster_index, int16_t mode, uint8_t test_line_of_fire, int ignore_object_index, uint8_t ignore_vehicles);
extern uint8_t actor_is_noncombat(int actor_index);
extern uint8_t actor_in_combat(int actor_index);
extern uint8_t actor_is_fighting(int actor_index);
extern int16_t actor_get_perception_knowledge(int actor_index, int prop_index);
extern int16_t actor_visibility_at_point(int actor_index, actor_position_data *sense_position, const real_point3d *position, char lighting, int16_t line_of_sight, uint8_t use_frustum, uint8_t store_debugging_information, int16_t combat_knowledge_type);
extern int16_t actor_audibility_at_point(int actor_index, actor_position_data *sense_position, const real_point3d *position, const location *location, int16_t sound_volume, float perception_factor, int16_t line_of_sight);
extern void *datum_try_and_get(const data_array *data, int index);
extern int actor_perception_assess_vehicle_danger(int actor_index, int vehicle_index, actor_position_data *sense_position, uint8_t currently_perceived);
extern int actor_perception_assess_suicide_danger(int actor_index, int unit_index, float suicide_radius, float current_distance, uint8_t enemy, uint8_t currently_perceived);
extern uint8_t actor_has_ranged_weapon(int actor_index);
extern void actor_perception_unreachable(int actor_index, int prop_index, uint8_t unreachable);
extern uint8_t actor_perception_desire_prop(int actor_index, int16_t desired_target_state, int unit_index, int unit_actor_index, uint8_t in_use, uint8_t player, uint8_t enemy, uint8_t dead, int16_t dead_ticks, float suicide_radius, float distance_squared, int16_t required_ticks, uint8_t *optional_reference);
extern uint8_t actor_compute_prop_unopposable(int actor_index, int prop_index);
extern float actor_compute_prop_target_weight(int actor_index, int prop_index);
extern float actor_look_compute_prop_interest(int actor_index, int prop_index);

void prop_status_refresh(int actor_index, int prop_index, actor_position_data *sense_position)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( !actor->meta.active )
        return;

    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);
    encounter_datum *encounter = (actor->meta.encounter_index == -1)
                    ? 0
                    : DATUM_GET(encounter_data, encounter_datum, actor->meta.encounter_index);
    prop_datum *prop = DATUM_GET(prop_data, prop_datum, prop_index);
    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, prop->unit_index)->datum);
    int now = game_time_get();

    char standdown;
    if ( (encounter && encounter->blind) || (standdown = 0, actor->state.mode == _actor_mode_asleep) )
        standdown = 1;

    int is_enemy = prop->enemy;
    prop->ignore = (unit_object->unit.flags & (1u << _unit_ignored_by_actors_bit)) != 0;
    if ( is_enemy )
    {
        prop->preferred_target = (unit_object->unit.flags & (1u << _unit_preferred_target_bit)) != 0;
        int targeting_mode = (int)(uint16_t)actor->external_orders.desired_target_type;
        if ( targeting_mode == _desired_target_ai )
        {
            if ( prop->actor_index != -1 )
            {
                unsigned int target = (unsigned int)actor->external_orders.desired_target_ai_index;
                if ( target != -1 )
                {
                    actor_datum *target_actor = DATUM_GET(actor_data, actor_datum,
                                                                   prop->actor_index);
                    if ( (uint16_t)target_actor->meta.encounter_index == (uint16_t)target )
                    {
                        if ( target >> 30 )
                        {
                            int16_t expected_role;
                            if ( target >> 30 == 1 )
                            {
                                expected_role = target_actor->meta.squad_index;
                            }
                            else
                            {
                                if ( target >> 30 == 3 )
                                    goto done_target;
                                expected_role = target_actor->meta.platoon_index;
                            }
                            if ( ((uint8_t *)&actor->external_orders.desired_target_ai_index)[1] != expected_role )
                                goto done_target;
                        }
                        prop->preferred_target = 1;  /* LABEL_23 */
                    }
                }
            }
        }
        else if ( targeting_mode == _desired_target_player && prop->player )
        {
            prop->preferred_target = 1;
        }
    }
done_target:;

    int16_t prev_speed = prop->quantized_speed;
    real_vector3d velocity;
    object_get_velocities(prop->unit_index, &velocity, 0);
    float speed = __fsqrts((velocity.n[0] * velocity.n[0])
                           + ((velocity.n[1] * velocity.n[1]) + (velocity.n[2] * velocity.n[2])));
    if ( speed >= 0.0033333334f )
    {
        if ( speed >= 0.016666668f )
            prop->quantized_speed = (speed >= SECONDS_PER_TICK) ? 3 : 2;
        else
            prop->quantized_speed = 1;
    }
    else
    {
        prop->quantized_speed = 0;
    }

    /* closing speed: component of (unit velocity - sensor velocity) along actor->prop, negated */
    float closing = -(((velocity.n[0] - sense_position->velocity.n[0]) * prop->actor_to_prop.n[0])
                      + ((prop->actor_to_prop.n[1] * (velocity.n[1] - sense_position->velocity.n[1]))
                         + ((velocity.n[2] - sense_position->velocity.n[2]) * prop->actor_to_prop.n[2])));
    if ( closing >= -0.033333335f )
    {
        if ( closing >= -0.016666668f )
        {
            if ( closing >= -0.0033333334f )
            {
                if ( closing >= 0.0033333334f )
                {
                    if ( closing >= 0.016666668f )
                    {
                        char q = 5;
                        if ( closing >= SECONDS_PER_TICK )
                            q = 6;
                        prop->quantized_closing_speed = q;
                    }
                    else
                    {
                        prop->quantized_closing_speed = 4;
                    }
                }
                else
                {
                    prop->quantized_closing_speed = 3;
                }
            }
            else
            {
                prop->quantized_closing_speed = 2;
            }
        }
        else
        {
            prop->quantized_closing_speed = 1;
        }
    }
    else
    {
        prop->quantized_closing_speed = 0;
    }

    int state = prop->state;
    if ( state >= _prop_state_becoming_unacknowledged && state <= _prop_state_acknowledged && prev_speed <= 1 && prop->quantized_speed > 1 )
    {
        direction_specification glance;
        direction_specification empty;
        glance = *direction_get_empty(&empty);
        glance.type = _direction_specification_prop;
        glance.___u1.prop_index = prop_index;
        actor_look_secondary(actor_index, _secondary_look_started_moving_prop, _secondary_look_priority_default, &glance);
    }

    /* quantized distance */
    float distance = prop->distance;
    if ( distance >= 1.0f )
    {
        if ( distance >= 6.0f )
        {
            if ( distance >= 10.0f )
                prop->quantized_distance = (distance >= 30.0f) ? 4 : 3;
            else
                prop->quantized_distance = 2;
        }
        else
        {
            prop->quantized_distance = 1;
        }
    }
    else
    {
        prop->quantized_distance = 0;
    }

    /* quantized facing from the unit's aiming vector vs the prop->actor direction */
    real_vector3d aiming;
    unit_get_aiming_vector(prop->unit_index, &aiming);
    float facing = -((aiming.n[0] * prop->actor_to_prop.n[0])
                     + ((prop->actor_to_prop.n[1] * aiming.n[1]) + (aiming.n[2] * prop->actor_to_prop.n[2])));
    float lateral;
    if ( facing > 0.0f )
        lateral = (facing < 1.0f)
                ? (__fsqrts(-((facing * facing) - 1.0f)) * prop->distance)
                : 0.0f;
    else
        lateral = 3.4028235e38f;

    if ( facing > 0.99250001f || lateral < 0.5f )
        prop->quantized_facing = 0;
    else if ( facing > 0.90630001f || lateral < 1.5f )
        prop->quantized_facing = 1;
    else if ( facing <= 0.5f )
        prop->quantized_facing = (facing <= 0.0f) ? 4 : 3;
    else
        prop->quantized_facing = 2;

    int current_state = prop->state;
    /* cntlzw(x-1) & 0x20 <=> x == 1: unit effect 1 is "shooting" */
    prop->shooting = (_cntlzw((uint16_t)prop->unit_effect - 1) & 0x20) != 0;

    if ( current_state < _prop_state_uninspected_orphan || current_state > _prop_state_inspected_orphan )
    {
        char noticed = 0;
        int16_t los_mode;
        if ( !prop->player || (los_mode = 2, !prop->enemy) )
            los_mode = 0;
        prop->line_of_sight = ai_test_line_of_sight(
            &sense_position->head_position,
            sense_position->body_location.cluster_index,
            &prop->head_position,
            prop->body_location.cluster_index,
            los_mode,
            0,
            prop->vehicle_index,
            actor->input.vehicle_index != -1);  /* deviation: v5[344] terms cancel to this != -1 test */
        prop->lighting = 2;
        if ( unit_object->object.type != object_type_biped )
        {
            prop->flying = 0;
        }
        else
        {
            biped_definition *biped_def = TAG_GET(biped_definition, unit_object->definition_index);
            prop->flying = (biped_def->biped.flags & (1u << _biped_flying_bit)) != 0;
        }
        prop->active_camouflage = unit_object->unit.active_camouflage > 0.5f;
        prop->flashlight = (unit_object->unit.flags & (1u << _unit_integrated_light_on_bit)) != 0;

        /* death tracking (prior pass mislabeled these as visibility): damage_flags bit 2 = dead;
         * really dead only if not feigning death */
        int dead_now = (unit_object->object.damage_flags >> _object_dead_bit) & 1;
        char really_dead_now;
        if ( !dead_now || (really_dead_now = 1, unit_object->unit.feign_death_timer) )
            really_dead_now = 0;
        char just_killed_now;
        if ( !dead_now || (just_killed_now = 1, prop->dead) )
            just_killed_now = 0;
        prop->really_dead = really_dead_now;
        prop->dead = dead_now;
        prop->just_killed = just_killed_now;
        if ( just_killed_now && !prop->enemy && actor->state.mode < _actor_mode_combat )
            noticed = 1;  /* an ally just died in front of us */
        if ( dead_now )
            prop->required_ticks = 0;

        /* track represented unit actor (swarm controller preferred over plain controller) */
        int unit_actor = unit_object->unit.swarm_actor_index;
        char from_swarm;
        if ( unit_actor == -1 )
        {
            unit_actor = unit_object->unit.actor_index;
            from_swarm = 0;
        }
        else
        {
            from_swarm = 1;
        }
        if ( unit_actor != prop->actor_index )
        {
            int orphan = prop->___u3.orphan_prop_index;
            prop->actor_index = unit_actor;
            prop->swarm = from_swarm;
            if ( orphan != -1 )
            {
                prop_datum *orphan_prop = DATUM_GET(prop_data, prop_datum, orphan);
                orphan_prop->actor_index = unit_actor;
                orphan_prop->swarm = prop->swarm;
            }
        }

        uint8_t is_noncombat, is_in_combat, is_fighting;
        if ( unit_actor == -1 )
        {
            is_noncombat = 0;
            is_in_combat = 0;
            is_fighting = prop->dead == 0;
        }
        else
        {
            is_noncombat = actor_is_noncombat(prop->actor_index);
            is_in_combat = actor_in_combat(prop->actor_index);
            is_fighting = actor_is_fighting(prop->actor_index);
        }
        if ( is_in_combat && !prop->in_combat && !prop->enemy && actor->state.mode < _actor_mode_combat )
            noticed = 1;  /* an ally just entered combat */
        prop->noncombat = is_noncombat;
        prop->fighting = is_fighting;
        prop->in_combat = is_in_combat;

        if ( noticed )
        {
            int16_t visibility = 0;
            if ( !standdown )
            {
                int16_t knowledge = actor_get_perception_knowledge(actor_index, prop_index);
                /* lighting re-derived from disasm 0x837DAC0C-24: flashlight forces full-bright (2),
                 * else the prop's cached lighting byte (was a bogus uninitialized local). */
                char lighting = prop->flashlight ? 2 : prop->lighting;
                uint8_t use_frustum = 1;
                uint8_t store_debugging_information = 0;
                visibility = actor_visibility_at_point(actor_index, sense_position,
                                                       &prop->head_position, lighting,
                                                       prop->line_of_sight, use_frustum,
                                                       store_debugging_information, knowledge);
            }
            if ( visibility < 2 )
            {
                prop->perception = visibility;
                prop->visibility = visibility;
                prop->state = _prop_state_unacknowledged;
            }
        }

        if ( prop->ignore )
        {
            prop->perception = 0;
            prop->ineffability = 0;
            prop->audibility = _ai_sound_volume_silent;
            prop->visibility = 0;
        }
        else
        {
            char force_unseen = standdown;
            if ( prop->active_camouflage )
            {
                if ( prop->enemy )
                    force_unseen = 1;
                else if ( !prop->player || (force_unseen = 1, prop->distance <= 4.0f) )
                    force_unseen = 0;
            }
            if ( force_unseen )
            {
                prop->just_became_visible = 0;
                prop->visibility = 0;
            }
            else
            {
                /* use_frustum re-derived from disasm 0x837DAD00-0x837DAD90 (was a bogus uninitialized
                 * local): flying drivers and mounted weapons never frustum-cull; for an enemy prop the
                 * frustum test is dropped while its perception state sits in the becoming/acknowledged
                 * band (2..3, subfc/subfe mask); for a non-enemy prop it applies only pre-combat and
                 * only when the prop is dead or fighting. */
                uint8_t use_frustum = 1;
                if ( actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying )
                    use_frustum = 0;
                else if ( actor->meta.type == _actor_type_mounted_weapon )
                    use_frustum = 0;
                else if ( prop->enemy )
                {
                    if ( prop->state >= _prop_state_becoming_unacknowledged )
                        use_frustum = prop->state > _prop_state_acknowledged;
                }
                else
                {
                    use_frustum = actor->state.mode < _actor_mode_combat && (prop->dead || prop->in_combat);
                }
                char lighting2 = prop->flashlight ? 2 : prop->lighting;
                int16_t knowledge = actor_get_perception_knowledge(actor_index, prop_index);
                int16_t vis = actor_visibility_at_point(actor_index, sense_position,
                                                        &prop->head_position, lighting2,
                                                        prop->line_of_sight, use_frustum, prop->player,
                                                        knowledge);
                char newly_seen;
                if ( prop->visibility || (newly_seen = 1, vis <= 0) )
                    newly_seen = 0;
                prop->visibility = vis;
                prop->just_became_visible = newly_seen;
                if ( vis )
                {
                    prop->last_visible_head_position = prop->head_position;
                    prop->last_visible_time = now;
                }
            }

            if ( encounter && encounter->deaf )
            {
                prop->audibility = _ai_sound_volume_silent;
            }
            else
            {
                int sound_class = prop->unit_effect;
                if ( sound_class == _ai_unit_effect_shooting || sound_class == _ai_unit_effect_death_scream )
                {
                    prop->audibility = _ai_sound_volume_shout;
                }
                else
                {
                    int sound_unit = prop->vehicle_index;
                    if ( sound_unit == -1 )
                        sound_unit = prop->unit_index;
                    unit_datum *sound_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, sound_unit)->datum);
                    unit_definition *sound_def = TAG_GET(unit_definition, sound_object->definition_index);
                    /* DEVIATION: arg7 = r9 (float arg6 skips r8) = `lhz r9,0x38(r31)` @0x837DAE90 =
                     * line_of_sight; callee `mr r28,r9` @0x837D56B8 compares 0/1 @0x837D5828. */
                    prop->audibility = actor_audibility_at_point(actor_index, sense_position,
                        &prop->body_position, &prop->body_location, sound_def->unit.constant_sound,
                        1.0f, prop->line_of_sight);
                }
            }

            int sound_class2 = (uint16_t)prop->unit_effect;
            prop->ineffability = 0;
            if ( !sound_class2 )
                prop->ineffability = 3;
            if ( prop->flashlight && prop->quantized_facing <= 2 && prop->quantized_distance <= 2
              && (!prop->line_of_sight || prop->line_of_sight == _ai_line_of_sight_occluded) )
            {
                int extrasensory = prop->ineffability;
                if ( extrasensory <= 1 )
                    extrasensory = 1;
                prop->ineffability = extrasensory;
            }

            int16_t audibility = prop->audibility;
            int extrasensory = prop->ineffability;
            int best = audibility;
            if ( audibility <= extrasensory )
                best = prop->ineffability;
            if ( prop->visibility <= best )
            {
                if ( audibility > extrasensory )
                    best = prop->audibility;
            }
            else
            {
                best = prop->visibility;
            }
            prop->perception = best;
            if ( (int16_t)best == 1 )
            {
                int s = prop->state;
                if ( s >= _prop_state_becoming_unacknowledged && s <= _prop_state_acknowledged )
                    prop->perception = 2;
            }
        }

        if ( prop->perception )
        {
            prop->last_perceived_body_position = prop->body_position;
            prop->last_perceived_time = now;
        }

        int s2 = prop->state;
        if ( s2 >= _prop_state_becoming_unacknowledged && s2 <= _prop_state_acknowledged )
        {
            char promote = 0;
            actor_datum *witness;
            if ( prop->visibility >= 2
              || (prop->definitely_located
                  && prop->definite_knowledge_source_actor != -1
                  && (witness = datum_try_and_get(actor_data, prop->definite_knowledge_source_actor)) != 0
                  && witness->target.target_type >= actor_target_visible_enemy
                  && witness->target.target_prop_index != -1
                  && witness->orders.combat.shoot_at_target
                  && DATUM_GET(prop_data, prop_datum, witness->target.target_prop_index)->unit_index
                       == prop->unit_index) )
            {
                promote = 1;
            }
            if ( promote )
            {
                prop->ticks_since_definitely_located = 0;
                prop->definitely_located = 1;
            }
        }
    }
    else  /* corpse / state 4..5: cheaper visibility-only path */
    {
        int16_t los_mode;
        if ( !prop->player || (los_mode = 2, !prop->enemy) )
            los_mode = 0;
        int16_t los = ai_test_line_of_sight(
            &sense_position->head_position,
            sense_position->body_location.cluster_index,
            &prop->head_position,
            prop->body_location.cluster_index,
            los_mode,
            0,
            prop->vehicle_index,
            actor->input.vehicle_index != -1);
        char suppressed = prop->ignore;
        prop->line_of_sight = los;
        if ( suppressed || standdown )
        {
            prop->ineffability = 0;
            prop->perception = 0;
            prop->audibility = _ai_sound_volume_silent;
            prop->visibility = 0;
        }
        else
        {
            uint8_t use_frustum = 1;
            uint8_t store_debugging_information = 0;
            int16_t vis = actor_visibility_at_point(actor_index, sense_position,
                                                    &prop->head_position, prop->lighting, los,
                                                    use_frustum, store_debugging_information, 2);
            prop->visibility = vis;
            prop->audibility = _ai_sound_volume_silent;
            prop->ineffability = 0;
            prop->perception = vis;
        }
    }

    if ( prop->dangerous_vehicle_driver )
        actor_perception_assess_vehicle_danger(actor_index, prop->vehicle_index, sense_position,
                                               prop->perception >= 2);

    float suicide_radius = prop->suicide_radius;
    if ( suicide_radius > 0.0f && (prop->dead || unit_object->unit.animation.state == _unit_state_melee_attack) )
        actor_perception_assess_suicide_danger(actor_index, prop->unit_index, suicide_radius,
                                               prop->distance,
                                               prop->perception - 2 != 0,
                                               prop->perception >> 31);  /* sign bit of (promoted) perception */

    if ( prop->enemy )
    {
        int s = prop->state;
        if ( s >= _prop_state_becoming_unacknowledged && s <= _prop_state_acknowledged )
        {
            char unreachable = 0;
            if ( (actor_has_ranged_weapon(actor_index) && prop->distance < (double)actor->control.weapon_maximum_range)
              || ((character->flags & (1u << _actor_definition_suicidal_melee_attack_bit)) != 0
                  && prop->distance < (double)character->berserk.melee_attack_range) )
                unreachable = 1;
            if ( unreachable )
                actor_perception_unreachable(actor_index, prop_index, 0);
        }
    }

    int last_unreachable = prop->last_unreachable_time;
    if ( last_unreachable != -1 && last_unreachable + 150 < now )
        actor_perception_unreachable(actor_index, prop_index, 0);

    if ( prop->delay_requirement_decision )
    {
        if ( !actor_perception_desire_prop(actor_index, -1, prop->unit_index, prop->actor_index,
                                           prop->in_use, prop->player, prop->enemy, prop->dead,
                                           prop->dead_ticks,
                                           prop->suicide_radius,
                                           prop->distance * prop->distance,
                                           0, 0) )  /* deviation: required_ticks/optional_reference are r16==0 */
            prop->required_ticks = 0;
        prop->delay_requirement_decision = 0;
    }

    prop->unopposable_enemy = actor_compute_prop_unopposable(actor_index, prop_index);
    prop->target_weight = actor_compute_prop_target_weight(actor_index, prop_index);
    prop->look_interest = actor_look_compute_prop_interest(actor_index, prop_index);
    prop->refresh_stimuli = 1;
}
