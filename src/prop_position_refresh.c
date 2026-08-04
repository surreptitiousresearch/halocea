/* prop_position_refresh @0x837D5C88 — refresh the cached positional state of a prop from its unit object:
 * decides whether a position update is warranted (forced, live prop, or a dead-but-settled corpse that hasn't
 * been "cheated" yet), optionally re-selects the represented unit from a swarm (every 90 ticks), then samples
 * head/origin/center-of-mass, velocity, BSP location, underwater flag, vehicle seat (gunner/dangerous-driver)
 * and attached-child counts. Finally it recomputes the actor->prop direction vector via
 * actor_perception_find_sense_position. 2026-07-13: fully typed; the unit_from_swarm call now uses the
 * corrected true-role signature (see actor_perception_unit_from_swarm.c) — binary-verified
 * (r3 = &actor->input.position, r4 = prop->actor_index, r5 = prop->unit_index, r6 = 0). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_position_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/object_damage_flags.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_type.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
#include "headers/real_point3d.h"
extern int game_time_get(void);
extern int actor_perception_unit_from_swarm(actor_position_data *sense_position, int actor_index, int preferred_unit_index, uint8_t mark_units);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void unit_get_center_of_mass(int unit_index, real_point3d *center_of_mass);
extern int object_get_ultimate_parent(int object_index);
extern uint8_t scenario_location_underwater(const location *location, const real_point3d *position, int16_t *optional_weather_palette_index);
extern uint8_t vehicle_causes_collision_damage(uint16_t vehicle_index);
extern float normalize3d(real_vector3d *v);
extern void actor_perception_find_sense_position(uint16_t actor_index, const real_point3d *position, int prop_index, actor_position_data *sense_position);

void prop_position_refresh(int actor_index, int prop_index, actor_position_data *sense_position,
                           uint8_t forced_position_update, uint8_t full_status_update)
{
    actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);
    if ( !actor->meta.active )
        return;

    prop_datum *prop = DATUM_GET(prop_data, prop_datum, prop_index);
    unsigned __int8 do_update = 0;
    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, prop->unit_index)->datum);

    __int16 state = prop->state;
    if ( forced_position_update || state < _prop_state_uninspected_orphan || state > _prop_state_inspected_orphan )
    {
        do_update = 1;
    }
    else if ( !prop->orphan_corpse_cheated )
    {
        char settled_corpse;
        if ( (unit_object->object.damage_flags & (1u << _object_dead_bit)) == 0
          || unit_object->unit.feign_death_timer
          || prop->perception
          || (settled_corpse = 1,
              (unit_object->object.translational_velocity.n[2] * unit_object->object.translational_velocity.n[2])
                + (unit_object->object.translational_velocity.n[0] * unit_object->object.translational_velocity.n[0])
                + (unit_object->object.translational_velocity.n[1] * unit_object->object.translational_velocity.n[1]) >= 0.010000001f) )
        {
            settled_corpse = 0;
        }
        do_update = settled_corpse;
        if ( actor->target.target_prop_index == prop_index && (!prop->tried_to_uncover || !prop->tried_to_search) )
            do_update = 0;
        if ( do_update )
        {
            prop->orphan_corpse_cheated = 1;
            prop->dead = 1;
        }
    }

    if ( do_update )
    {
        if ( prop->swarm && prop->actor_index != -1 && full_status_update )
        {
            int now = game_time_get();
            if ( prop->swarm_unit_selected_time + 90 <= now )
            {
                prop->swarm_unit_selected_time = now;
                int new_unit = actor_perception_unit_from_swarm(&actor->input.position, prop->actor_index,
                                                                prop->unit_index, 0); /* phantom 5th arg dropped */
                if ( new_unit != prop->unit_index )
                {
                    prop->unit_index = new_unit;
                    unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, new_unit)->datum);
                    if ( prop->___u3.orphan_prop_index != -1 )
                        DATUM_GET(prop_data, prop_datum, prop->___u3.orphan_prop_index)->unit_index = new_unit;
                }
            }
        }

        unit_get_head_position(prop->unit_index, &prop->head_position);
        object_get_origin(prop->unit_index, &prop->body_position);
        unit_get_center_of_mass(prop->unit_index, &prop->center_of_mass);
        int unit_index = prop->unit_index;
        prop->velocity.n[0] = unit_object->object.translational_velocity.n[0];
        prop->velocity.n[1] = unit_object->object.translational_velocity.n[1];
        prop->velocity.n[2] = unit_object->object.translational_velocity.n[2];
        prop->pathfinding_surface_index = -1;

        unit_datum *ultimate_parent = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_get_ultimate_parent(unit_index))->datum);
        prop->body_location = ultimate_parent->object.location;
        prop->underwater = scenario_location_underwater(&prop->body_location, &prop->center_of_mass, 0);
        prop->vehicle_index = -1;
        prop->vehicle_gunner = 0;
        prop->dangerous_vehicle_driver = 0;
        prop->attached_to_unit_index = -1;

        int parent = unit_object->object.parent_object_index;
        if ( parent != -1 )
        {
            unit_datum *parent_unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent)->datum);
            int parent_type = parent_unit->object.type;
            if ( parent_type == object_type_vehicle )
            {
                int this_unit = prop->unit_index;
                char gunner;
                prop->vehicle_index = parent;
                if ( parent_unit->unit.gunner_object_index == this_unit || (gunner = 0, prop->type == _actor_type_mounted_weapon) )
                    gunner = 1;
                prop->vehicle_gunner = gunner;
                char dangerous;
                if ( parent_unit->unit.driver_object_index != this_unit
                  || (dangerous = 1, !vehicle_causes_collision_damage(prop->vehicle_index)) )
                    dangerous = 0;
                prop->dangerous_vehicle_driver = dangerous;
            }
            else if ( ((1 << parent_type) & object_mask_unit) != 0 )
            {
                prop->attached_to_unit_index = parent;
            }
        }

        prop->child_units_attached = 0;
        for ( int child = unit_object->object.first_child_object_index; child != -1; )
        {
            unit_datum *child_unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child)->datum);
            if ( ((1 << child_unit->object.type) & object_mask_unit) != 0 )
                ++prop->child_units_attached;
            child = child_unit->object.next_object_index;
        }
    }

    actor_perception_find_sense_position(actor_index, &prop->body_position, prop_index, sense_position);
    prop->actor_to_prop.n[0] = prop->body_position.n[0] - sense_position->body_position.n[0];
    prop->actor_to_prop.n[1] = prop->body_position.n[1] - sense_position->body_position.n[1];
    prop->actor_to_prop.n[2] = prop->body_position.n[2] - sense_position->body_position.n[2];
    float length = normalize3d(&prop->actor_to_prop);
    prop->distance = length;
    if ( length == 0.0f )
        prop->actor_to_prop = *global_forward3d;
}
