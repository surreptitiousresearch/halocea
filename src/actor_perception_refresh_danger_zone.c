/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* actor_perception_refresh_danger_zone @0x837D9348 — refreshes an actor's model of the nearest "danger"
 * object it is tracking (a live grenade / thrown prop / dangerous device). Re-reads the danger object's
 * origin and velocity, extrapolates its position 45 ticks ahead, rebuilds the danger's current point,
 * predicted point, midpoint, distance and bounding radius, then — per danger type (1 = held/thrown prop,
 * 2 = timed device, 3 = projectile) — decides whether the actor has "reacted" to it (via prop activity,
 * line-of-sight, visibility and audibility tests). If it just started reacting, it flicks a secondary look
 * toward the danger. The result flag is latched into the actor's behaviour payload.
 *
 * The actor record and the danger-object datum are addressed as raw byte bases (matching the decompiler,
 * which never had the full actor_datum/object payload types applied); accesses land in the opaque
 * behaviour payload so they are reproduced as raw offset casts per the actor_datum convention. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/actor_position_data.h"
#include "headers/direction_specification.h"
#include "headers/location.h"
#include "headers/object_datum.h"
#include "headers/projectile_datum.h"
#include "headers/unit_datum.h"
#include "headers/projectile_definition.h"
#include "headers/vehicle_definition.h"
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/prop_datum.h"
#include "headers/object_type.h"
#include "headers/unit_animation_state.h"
#include "headers/direction_specification_type.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/actor_knowledge.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern int object_get_ultimate_parent(int object_index);
extern void actor_perception_find_sense_position(uint16_t actor_index, const real_point3d *position, int prop_index, actor_position_data *sense_position);
extern int prop_get_active_by_unit_index(uint16_t actor_index, int unit_index);
extern int16_t unit_get_animation_frames_remaining(int unit_index, short *animation_state);
extern int16_t ai_test_line_of_sight(const real_point3d *p0, int16_t p0_cluster_index, const real_point3d *p1, int16_t p1_cluster_index, int16_t mode, uint8_t test_line_of_fire, int ignore_object_index, uint8_t ignore_vehicles);
extern int16_t actor_visibility_at_point(int actor_index, actor_position_data *sense_position, const real_point3d *position, char lighting, int16_t line_of_sight, uint8_t use_frustum, uint8_t store_debugging_information, int16_t combat_knowledge_type);
extern int16_t actor_audibility_at_point(uint16_t actor_index, actor_position_data *sense_position, const real_point3d *position, const location *location, int16_t sound_volume, float perception_factor, int16_t line_of_sight);
extern uint8_t actor_look_secondary(uint16_t actor_index, int16_t type, int16_t priority, direction_specification *direction);
extern direction_specification *direction_get_empty(direction_specification *result);
extern float __fsqrts(float x);

void actor_perception_refresh_danger_zone(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->danger_zone.danger_type <= actor_danger_zone_none ) /* danger type/count word — nothing tracked */
        return;

    int danger_object_index = actor->danger_zone.object_index;
    object_datum *danger_object = object_try_and_get_and_verify_type(danger_object_index, object_mask_all);
    if ( !danger_object )
    {
        actor->danger_zone.danger_type = actor_danger_zone_none;
        return;
    }

    /* re-sample the danger object's origin and where the actor would sense it from */
    object_get_origin(danger_object_index, &actor->danger_zone.position);
    actor_position_data sense[2];
    actor_perception_find_sense_position(actor_index, &actor->danger_zone.position, -1, sense);
    float sense_x = sense[0].body_position.n[0];
    float sense_y = sense[0].body_position.n[1];
    float sense_z = sense[0].body_position.n[2];

    char audible = 0;
    char reacted = 0;

    /* copy the danger object's velocity into the actor's danger record (object+104) */
    actor->danger_zone.velocity.n[0] = danger_object->object.translational_velocity.n[0];
    actor->danger_zone.velocity.n[1] = danger_object->object.translational_velocity.n[1];
    actor->danger_zone.velocity.n[2] = danger_object->object.translational_velocity.n[2];

    /* distance from the danger point to the actor's sense position */
    float to_sense_y = actor->danger_zone.position.n[1] - sense_y;
    float to_sense_z = actor->danger_zone.position.n[2] - sense_z;
    float to_sense_x = actor->danger_zone.position.n[0] - sense_x;
    actor->danger_zone.current_distance_from_actor =
        __fsqrts(to_sense_x * to_sense_x + (to_sense_y * to_sense_y + to_sense_z * to_sense_z));

    /* predicted point = current point + velocity * 45 ticks */
    float predicted_x = actor->danger_zone.velocity.n[0] * 45.0f + actor->danger_zone.position.n[0];
    actor->danger_zone.predict_danger_position.n[0] = predicted_x;
    actor->danger_zone.predict_danger_position.n[1] = actor->danger_zone.velocity.n[1] * 45.0f + actor->danger_zone.position.n[1];
    actor->danger_zone.predict_danger_position.n[2] = actor->danger_zone.velocity.n[2] * 45.0f + actor->danger_zone.position.n[2];

    /* midpoint between the current and predicted points */
    float mid_x = (actor->danger_zone.position.n[0] + predicted_x) * 0.5f;
    actor->danger_zone.bounding_sphere_center.n[0] = mid_x;
    float mid_y = (actor->danger_zone.position.n[1] + actor->danger_zone.predict_danger_position.n[1]) * 0.5f;
    actor->danger_zone.bounding_sphere_center.n[1] = mid_y;
    float mid_z = (actor->danger_zone.position.n[2] + actor->danger_zone.predict_danger_position.n[2]) * 0.5f; /* predict_danger_position z */
    actor->danger_zone.bounding_sphere_center.n[2] = mid_z;

    int danger_type = actor->danger_zone.danger_type;

    /* danger radius = half-extent from the midpoint plus the actor's radius bias (danger_radius) */
    float rad_y = actor->danger_zone.position.n[1] - mid_y;
    float rad_z = actor->danger_zone.position.n[2] - mid_z;
    float rad_x = actor->danger_zone.position.n[0] - mid_x;
    actor->danger_zone.bounding_sphere_radius =
        __fsqrts(rad_x * rad_x + (rad_y * rad_y + rad_z * rad_z)) + actor->danger_zone.danger_radius;

    uint8_t danger_moved_to_owner_team = 0; /* v21 */

    /* ignore_vehicles argument shared by the line-of-sight tests: the actor+344 terms cancel to
     * `1 - (actor+86 == -1)`, i.e. whether the actor's slot-86 index is valid. */
    uint8_t los_ignore_vehicles = (uint8_t)(actor->input.vehicle_index != -1);

    switch ( danger_type )
    {
        case actor_danger_zone_suicide: /* suicide unit (held / thrown prop carrier) */
        {
            reacted = actor->danger_zone.currently_perceived;
            if ( !reacted )
            {
                int active_prop = prop_get_active_by_unit_index(actor_index, danger_object_index);
                if ( active_prop != -1 )
                    reacted = (DATA_ARRAY_ELEMENT(prop_data, prop_datum, active_prop))->perception >= 2;
            }
            int16_t animation_state[8];
            int16_t frames_remaining = unit_get_animation_frames_remaining(danger_object_index, animation_state);
            if ( animation_state[0] != _unit_state_dying )
                frames_remaining = -1;
            actor->danger_zone.___u19.suicide.time_until_death = frames_remaining; /* recovered: scalar assign -> union member (danger_type 1) */
            break;
        }

        case actor_danger_zone_projectile: /* thrown grenade / timed device (fuse fraction 1-[144])/[145] */
        {
            int actor_unit_index = actor->meta.unit_index;
            if ( actor_unit_index != -1 )
                danger_moved_to_owner_team = danger_object->object.parent_object_index == actor_unit_index;

            projectile_datum *danger_projectile = (projectile_datum *)danger_object;
            if ( danger_projectile->projectile.detonation_timer <= 0.0f      /* proj+576 */
              || danger_projectile->projectile.detonation_timer_delta <= 0.0f )  /* proj+580 */
            {
                actor->danger_zone.___u19.projectile.time_until_explosion = -1; /* recovered: scalar assign -> union member (danger_type 2) */
            }
            else
            {
                direction_specification fuse;
                fuse.___u1.prop_index = (int)((1.0f - danger_projectile->projectile.detonation_timer)
                                               / danger_projectile->projectile.detonation_timer_delta);
                actor->danger_zone.___u19.projectile.time_until_explosion = (int16_t)fuse.___u1.object_index; /* recovered: scalar assign -> union member; low word of fuse frame count */
            }

            char already = 0;
            if ( actor->danger_zone.currently_perceived || danger_moved_to_owner_team )
                already = 1;
            reacted = already;
            if ( !already )
            {
                encounter_datum *encounter = (actor->meta.encounter_index == -1)
                        ? (encounter_datum *)0
                        : DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, actor->meta.encounter_index);
                if ( actor->state.mode != _actor_mode_asleep
                  && (!encounter || !encounter->blind)
                  && actor->danger_zone.current_distance_from_actor
                        < (double)(TAG_GET(projectile_definition, danger_object->definition_index))->projectile.danger_perception_radius )  /* def+412 */
                {
                    int16_t target_cluster = danger_object->object.location.cluster_index;  /* object+156 */
                    if ( danger_object->object.parent_object_index != -1 )
                        target_cluster = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_get_ultimate_parent(danger_object_index))->datum)->object.location.cluster_index;

                    int16_t line_of_sight = ai_test_line_of_sight(&sense[0].head_position,
                            sense[0].body_location.cluster_index, &actor->danger_zone.position,
                            target_cluster, 0, 0, danger_object_index, los_ignore_vehicles);

                    int16_t combat_knowledge;
                    if ( actor->state.combat_status < _actor_combat_status_investigate )
                        combat_knowledge = (actor->state.mode >> 15) + ((unsigned int)actor->state.mode >= _actor_mode_combat);
                    else
                        combat_knowledge = _actor_knowledge_searching;

                    if ( actor_visibility_at_point(actor_index, sense, &actor->danger_zone.position, 0,
                                                   line_of_sight, 1, 0, combat_knowledge) >= 2 )
                        reacted = 1;
                }
            }
            break;
        }

        case actor_danger_zone_vehicle: /* oncoming vehicle — reacts on sight or sound if it is moving fast enough and close enough */
        {
            vehicle_definition *danger_tag = TAG_GET(vehicle_definition, danger_object->definition_index);
            float speed_sq = danger_object->object.translational_velocity.n[2] * danger_object->object.translational_velocity.n[2]
                           + (danger_object->object.translational_velocity.n[0] * danger_object->object.translational_velocity.n[0]
                              + danger_object->object.translational_velocity.n[1] * danger_object->object.translational_velocity.n[1]);
            if ( speed_sq < 0.000044444445f
              || actor->danger_zone.current_distance_from_actor > (double)(danger_tag->object.bounding_radius + 10.0f) )  /* def+4 */
            {
                actor->danger_zone.danger_type = actor_danger_zone_none;
                break;
            }

            reacted = actor->danger_zone.currently_perceived;
            if ( reacted )
                break;

            int owner_unit = ((unit_datum *)danger_object)->unit.driver_object_index;  /* unit+804 */
            int active_prop;
            if ( owner_unit != -1 && (active_prop = prop_get_active_by_unit_index(actor_index, owner_unit)) != -1 )
            {
                reacted = (DATA_ARRAY_ELEMENT(prop_data, prop_datum, active_prop))->perception >= 2;
                break;
            }

            encounter_datum *encounter = (actor->meta.encounter_index == -1)
                    ? (encounter_datum *)0
                    : DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, actor->meta.encounter_index);
            if ( actor->state.mode == _actor_mode_asleep || (encounter && encounter->blind) )
                audible = 1;

            const location *danger_location;
            if ( danger_object->object.parent_object_index == -1 )
                danger_location = &danger_object->object.location; /* object_datum +152 */
            else
                danger_location = (const location *)(((int)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_get_ultimate_parent(danger_object_index))->datum) + 152);

            int16_t line_of_sight = ai_test_line_of_sight(&sense[0].head_position,
                    sense[0].body_location.cluster_index, &actor->danger_zone.position,
                    danger_location->cluster_index, 0, 0, danger_object_index, los_ignore_vehicles);

            int16_t combat_knowledge;
            if ( actor->state.combat_status < _actor_combat_status_investigate )
                combat_knowledge = (actor->state.mode >> 15) + ((unsigned int)actor->state.mode >= _actor_mode_combat);
            else
                combat_knowledge = _actor_knowledge_searching;

            if ( audible
              || actor_visibility_at_point(actor_index, sense, &actor->danger_zone.position, 0,
                                           line_of_sight, 1, 0, combat_knowledge) < 2 )
            {
                /* not clearly visible: fall back to hearing it */
                if ( actor_audibility_at_point(actor_index, sense, &actor->danger_zone.position,
                        danger_location, danger_tag->unit.constant_sound, 1.0f, line_of_sight) >= 2 )  /* def+386 */
                    reacted = 1;
            }
            else
            {
                /* clearly visible */
                reacted = 1;
            }
            break;
        }
    }

    if ( reacted && !actor->danger_zone.currently_perceived )
    {
        /* first tick of reaction: flick a secondary look toward the danger */
        direction_specification look;
        direction_specification scratch;
        look = *direction_get_empty(&scratch);
        look.type = _direction_specification_danger;
        actor_look_secondary(actor_index, _secondary_look_dangerous_object, _secondary_look_priority_default, &look);
    }

    actor->danger_zone.currently_perceived = reacted;
    actor->danger_zone.attached_to_us = danger_moved_to_owner_team;
}
