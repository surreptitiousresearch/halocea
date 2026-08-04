/* actor_input_update @ 0x8371EF28 — sample the actor's unit (or swarm) physical state into the actor datum at
 * the start of its update. Two paths:
 *   - Swarm actors: average the member units' positions/headings into the swarm datum, then sample the swarm's
 *     representative position.
 *   - Normal actors: sample the unit position, test underwater, resolve the vehicle seat the unit occupies
 *     (driver/gunner/passenger -> seat role at actor+350) and the seat's combat capabilities, migrate the actor
 *     to the vehicle's encounter/squad when required, scan the unit's threat list, and finally rebuild the
 *     actor's facing/aiming vectors and orientation basis (right/up via cross products) plus health/shield.
 *
 * 2026-07-14: fully typed (unit_datum/vehicle_datum/biped_datum/projectile_datum/swarm/encounter). */

#include <stdint.h>
#include <string.h>
#include "headers/actor_definition_flags.h"
#include "headers/projectile_datum_flags.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_position_data.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_definition.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/unit_datum.h"
#include "headers/projectile_datum.h"
#include "headers/biped_datum.h"
#include "headers/encounter_datum.h"
#include "headers/swarm_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/squad_datum.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/object_type.h"
#include "headers/vehicle_definition_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/actor_vehicle_driver_type.h"



#include "headers/location.h"
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void actor_input_sample_position(int actor_index, int unit_index, actor_position_data *position);
extern void unit_get_center_of_mass(int unit_index, real_point3d *center_of_mass);
extern uint8_t scenario_location_underwater(const location *location, const real_point3d *position, int16_t *optional_weather_palette_index);
extern actor_variant_definition *actor_combat_get_firing_variant_definition(int actor_index);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern void actor_change_encounter(int actor_index, int encounter_index, int16_t squad_index);
extern void unit_get_facing_vector(int unit_index, real_vector3d *facing_vector);
extern float normalize2d(real_vector2d *v);
extern float normalize3d(real_vector3d *v);

void actor_input_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);

    if ( actor->meta.swarm )                                       /* meta.swarm */
    {
        swarm_datum *swarm = DATUM_GET(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
        __int16 member_count = swarm->unit_count;
        swarm->swarm_center = *global_origin3d;                   /* zero the position accumulator */

        for ( int member = 0; member < swarm->unit_count; member = (__int16)(member + 1) )
        {
            int object_index = swarm->unit_indices[member];
            swarm_component_datum *component = DATUM_GET(swarm_component_data, swarm_component_datum,
                                                                  swarm->component_indices[member]);
            biped_datum *member_biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);

            int surface_index = -1;
            if ( !member_biped->object.type )
                surface_index = member_biped->biped.support_surface_index;
            object_get_origin(object_index, &component->position);
            component->surface_index = surface_index;

            swarm->swarm_center.x += component->position.x;
            swarm->swarm_center.y += component->position.y;
            swarm->swarm_center.z += component->position.z;
        }

        int count = swarm->unit_count;
        if ( count > 0 )
        {
            float inv = 1.0f / (float)count;
            swarm->swarm_center.x = swarm->swarm_center.x * inv;
            swarm->swarm_center.y = swarm->swarm_center.y * inv;
            swarm->swarm_center.z = swarm->swarm_center.z * inv;
        }

        memset(&actor->input.position, 0, sizeof(actor->input));
        int representative_object = actor->meta.swarm_unit_index;
        actor->input.vehicle_index = -1;
        actor->input.pathfinding_surface_index = -1;
        if ( representative_object != -1 )
            actor_input_sample_position(actor_index, representative_object,
                                        &actor->input.position);
        return;
    }

    int         unit_index  = actor->meta.unit_index;
    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    int         vehicle_index  = unit_object->object.parent_object_index;
    vehicle_datum *vehicle_object = (vehicle_index == -1)
                         ? 0
                         : ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);

    actor_input_sample_position(actor_index, unit_index, &actor->input.position);

    real_point3d center_of_mass;
    unit_get_center_of_mass(actor->meta.unit_index, &center_of_mass);
    /* recovered: (const location *)((char *)&actor->input.position + 36) -> input.position.body_location */
    actor->input.underwater = scenario_location_underwater(&actor->input.position.body_location, &center_of_mass, NULL);
    actor->state.flying = (character->flags & (1u << _actor_definition_flying_bit)) != 0;

    if ( vehicle_object && vehicle_object->object.type == object_type_vehicle )
    {
        vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, vehicle_object->definition_index);
        actor->input.vehicle_index = vehicle_index;
        actor->input.vehicle_gunner_bombardment = 0;
        actor->input.vehicle_gunner = 0;
        actor->input.vehicle_driver_type = 0;                      /* seat role */

        if ( vehicle_object->unit.driver_object_index == unit_index )
        {
            actor->input.vehicle_driver_type = _actor_vehicle_driver_unknown;
            int seat_flags = vehicle_def->vehicle.flags;
            if ( (seat_flags & (1u << _vehicle_ai_driver_enable_bit)) != 0 )
            {
                if ( (seat_flags & (1u << _vehicle_ai_driver_flying_bit)) != 0 )
                {
                    actor->state.flying = 1;
                    actor->input.vehicle_driver_type = _actor_vehicle_driver_directional_flying;
                }
                else if ( (seat_flags & (1u << _vehicle_ai_driver_nondirectional_bit)) != 0 )
                {
                    /* bit14 = _vehicle_ai_driver_hovering: hovering -> 2, non-hovering -> 3 */
                    actor->input.vehicle_driver_type = ((unsigned int)~seat_flags >> _vehicle_ai_driver_hovering_bit) & 1 | 2;
                }
            }
        }
        if ( vehicle_object->unit.gunner_object_index == unit_index )
        {
            actor->input.vehicle_gunner = 1;
            /* recovered: *(float *)((char *)variant + 0x14C) -> ranged_combat.weapon_bombardment_range */
            actor->input.vehicle_gunner_bombardment = actor_combat_get_firing_variant_definition(actor_index)->ranged_combat.weapon_bombardment_range > 0.0f;
        }

        actor->input.vehicle_passenger = actor->input.vehicle_driver_type <= _actor_vehicle_driver_unknown;

        __int16 vehicle_encounter = vehicle_object->unit.fake_encounter_index;
        if ( vehicle_encounter != -1 )
        {
            int  actor_encounter = actor->meta.encounter_index;
            char migrate = 0;
            if ( (unsigned __int16)actor_encounter == vehicle_encounter )
            {
                __int16 vehicle_squad = vehicle_object->unit.fake_squad_index;
                if ( vehicle_squad != -1 )
                {
                    __int16 actor_squad = actor->meta.disconnected_squad_index;
                    if ( actor_squad != vehicle_squad )
                    {
                        encounter_datum *encounter = DATUM_GET(encounter_data, encounter_datum,
                                                                        actor_encounter);
                        migrate = 1;
                        if ( encounter->follow_target_type > 0 )
                        {
                            __int16 squad_base = encounter->squad_base;
                            if ( squad_array[(__int16)(squad_base + actor_squad)].automatic_migration_target )
                                migrate = squad_array[(__int16)(vehicle_squad + squad_base)].automatic_migration_target == 0;
                        }
                    }
                }
            }
            else
            {
                migrate = 1;
            }

            if ( migrate )
            {
                if ( !actor->meta.stored_prevehicle_encounter )
                {
                    __int16 actor_squad = actor->meta.disconnected_squad_index;
                    actor->meta.prevehicle_encounter_index = actor_encounter;  /* remember original encounter */
                    actor->meta.stored_prevehicle_encounter = 1;
                    actor->meta.prevehicle_squad_index = actor_squad;          /* remember original squad */
                    if ( actor_encounter != -1 )
                        DATUM_GET(encounter_data, encounter_datum, actor_encounter)->is_prevehicle_encounter = 1;
                }
                actor_change_encounter(actor_index, vehicle_object->unit.fake_encounter_index,
                                       vehicle_object->unit.fake_squad_index);
            }
        }
    }
    else
    {
        char was_migrated = actor->meta.stored_prevehicle_encounter;
        actor->input.vehicle_index = -1;
        actor->input.vehicle_driver_type = 0;
        actor->input.vehicle_passenger = 0;
        actor->input.vehicle_gunner = 0;
        if ( was_migrated )
        {
            actor_change_encounter(actor_index, actor->meta.prevehicle_encounter_index, actor->meta.prevehicle_squad_index);
            actor->meta.stored_prevehicle_encounter = 0;
        }
    }

    int flaming = unit_object->unit.flaming_death_delay;
    actor->input.delayed_attached_projectile_index = -1;           /* preferred-target object */
    actor->input.melee_attacker_attached = 0;
    actor->input.burning_to_death = flaming != 0;

    for ( int threat = unit_object->object.first_child_object_index; threat != -1; )
    {
        projectile_datum *threat_object = ((projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, threat)->datum);
        if ( threat_object->object.type
          || !game_team_is_enemy(actor->meta.team_index, threat_object->object.owner_team_index) )
        {
            if ( threat_object->object.type == object_type_projectile
              && ((threat_object->projectile.flags & (1u << _projectile_will_super_explode_bit)) != 0
                  || (actor->danger_zone.danger_type == actor_danger_zone_projectile && threat == *(int *)&actor->danger_zone.position.x)) )
                actor->input.delayed_attached_projectile_index = threat;
        }
        else
        {
            actor->input.melee_attacker_attached = 1;
        }
        threat = threat_object->object.next_object_index;
    }

    actor->input.in_midair = 0;
    actor->input.pathfinding_surface_index = -1;
    if ( !unit_object->object.type && actor->input.vehicle_index == -1 )
    {
        biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->meta.unit_index)->datum);
        if ( biped->biped.airborne_ticks >= 6 )
            actor->input.in_midair = 1;
        actor->input.pathfinding_surface_index = biped->biped.pathfinding_surface_index;
        actor->input.pathfinding_point = biped->biped.pathfinding_point;
    }

    int facing_source = (actor->input.vehicle_driver_type <= 0) ? actor->meta.unit_index : actor->input.vehicle_index;
    unit_get_facing_vector(facing_source, &actor->input.facing_vector);
    if ( !actor->state.flying )
    {
        if ( normalize2d((real_vector2d *)&actor->input.facing_vector) <= 0.0f )
            actor->input.facing_vector = *global_forward3d;
        else
            actor->input.facing_vector.n[2] = 0.0f;
    }

    if ( actor->input.vehicle_gunner )                             /* gunner: aiming from the turret */
    {
        vehicle_datum *turret = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->input.vehicle_index)->datum);
        if ( (TAG_GET(vehicle_definition, turret->definition_index)->vehicle.flags & (1u << _vehicle_ai_weapon_cannot_rotate_bit)) != 0 )
        {
            unit_get_facing_vector(actor->meta.unit_index, &actor->input.aiming_vector);
        }
        else
        {
            actor->input.aiming_vector = turret->unit.aiming_vector;
        }
    }
    else
    {
        actor->input.aiming_vector = unit_object->unit.aiming_vector;
    }

    /* up vector and orientation basis */
    actor->input.looking_vector = unit_object->unit.looking_vector;

    /* right = up_world x forward (cross product), then normalized */
    actor->input.looking_left_vector.n[0] = actor->input.looking_vector.n[2] * global_up3d->n[1]
                                           - global_up3d->n[2] * actor->input.looking_vector.n[1];
    actor->input.looking_left_vector.n[2] = global_up3d->n[0] * actor->input.looking_vector.n[1]
                                           - actor->input.looking_vector.n[0] * global_up3d->n[1];
    actor->input.looking_left_vector.n[1] = global_up3d->n[2] * actor->input.looking_vector.n[0]
                                           - actor->input.looking_vector.n[2] * global_up3d->n[0];
    normalize3d(&actor->input.looking_left_vector);

    /* up = forward x right */
    actor->input.looking_up_vector.n[2] = actor->input.looking_left_vector.n[1] * actor->input.looking_vector.n[0]
                                         - actor->input.looking_left_vector.n[0] * actor->input.looking_vector.n[1];
    actor->input.looking_up_vector.n[0] = actor->input.looking_left_vector.n[2] * actor->input.looking_vector.n[1]
                                         - actor->input.looking_vector.n[2] * actor->input.looking_left_vector.n[1];
    actor->input.looking_up_vector.n[1] = actor->input.looking_vector.n[2] * actor->input.looking_left_vector.n[0]
                                         - actor->input.looking_left_vector.n[2] * actor->input.looking_vector.n[0];

    actor->input.body_vitality      = unit_object->object.body_vitality;
    actor->input.shield_vitality    = unit_object->object.shield_vitality;
    actor->input.recent_body_damage = unit_object->object.recent_body_damage;
    actor->input.recent_shield_damage = unit_object->object.recent_shield_damage;
}
