/* machine_update @0x837BBFC8 — per-frame update for one device_machine object (doors, lifts, powered gear).
 * Four stages run each tick:
 *   1. Type-2 (looping/rotating) machines advance a 0..1 position phase by an interpolated increment and wrap
 *      it at 1.0, publish the phase to the object's device group, and flag the object for repositioning.
 *   2. Auto-open doors (definition type 0) run a proximity test every 4th tick: if any non-deleted object is
 *      inside the activation sphere - and, when the machine's one-way flag is set and it is fully closed, is
 *      not a friendly unit standing in front of it - the door's device group is driven open and the auto-close
 *      timer is primed to -3.
 *   3. Auto-close: while a type-0 machine is fully open (phase == 1.0), count up an open timer; once it passes
 *      the definition's delay, drive the group closed. Any other phase resets the timer.
 *   4. Lift/elevator machines (definition flag 0x4) carry rider objects: measure how far the drive node moved
 *      since last tick and translate every object parented to this machine by that delta.
 * Finally, if the object was flagged for repositioning, move it to its stored position.
 *
 * The object datum and machine definition are accessed at raw byte offsets (char *), per project convention.
 *
 * DEVIATIONS:
 *  - objects_in_sphere is the FPR-shadow ABI trap (as in actor_move_avoidance_setup): the float radius is in
 *    f1 and reserves shadow GPR r7, so the decompiler's uninitialized `v7` is the dead shadow slot, not an
 *    argument. The real trailing args are object_indices (r8) and maximum_count (r9) = 16 and 2048. The second
 *    call's buffer is therefore ~8KB of ints; the decompiler under-sized it to 128 bytes.
 *  - The type-2 phase value is held as a float across the >= 1.0 wrap test (outer (float) cast in the source);
 *    (float) casts are preserved on the arithmetic to keep the PPC single-precision rounding. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/machine_datum.h"
#include "headers/biped_datum.h"
#include "headers/machine_definition.h"
#include "headers/device_group_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/object_damage_flags.h"
#include "headers/device_datum_flags.h"
#include "headers/machine_datum_flags.h"
#include "headers/machine_flags.h"
#include "headers/machine_type.h"
#include "headers/unit_definition_flags.h"
#include "headers/unit_definition.h"
#include "headers/blam_data_globals.h"
#include "headers/object_type.h"


extern int game_time_get(void);
extern int16_t objects_in_sphere(unsigned int class_flags, unsigned int type_flags, const location *location, const real_point3d *center, float radius, int *object_indices, int16_t maximum_count);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern uint8_t device_group_set_desired_value(int16_t group_index, float desired_value);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);

unsigned __int8 machine_update(int machine_index)
{
    machine_datum *machine = (machine_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, machine_index)->datum;
    machine_definition *definition = TAG_GET(machine_definition, machine->definition_index);
    device_group_datum *groups = (device_group_datum *)device_groups_data->data;

    /* --- stage 1: type-2 machines advance and wrap a 0..1 position phase --- */
    if ( definition->machine.type == _machine_gear )
    {
        float power = machine->device.power;
        float advanced = (((((float)1.0 - power) * definition->device.runtime_maximum_depowered_position_velocity)
                        + (definition->device.runtime_maximum_powered_position_velocity * power))
                + machine->device.position);
        machine->device.position = advanced;
        if ( advanced >= 1.0 )
            machine->device.position = advanced - (float)1.0;

        unsigned int object_flags = machine->device.flags;
        machine->device.position_velocity = 0.0f;
        unsigned __int16 group_index = (unsigned __int16)machine->device.position_group_index;
        machine->device.flags = object_flags | (1u << _device_animation_changed_bit);
        if ( group_index != 0xFFFF )
            groups[group_index].desired_value = machine->device.position;
    }

    /* --- stage 2: auto-open doors run a proximity test every 4th tick --- */
    if ( (machine->machine.flags & (1u << _machine_does_not_operate_automatically_bit)) == 0
      && definition->machine.type == _machine_door
      && ((game_time_get() + machine_index) & 3) == 0 )
    {
        float activation_radius = definition->device.automatic_activation_radius;
        unsigned __int8 blocked = 0;
        if ( activation_radius < 0.0001f )
            activation_radius = machine->object.bounding_sphere_radius;

        int nearby_objects[16];
        __int16 nearby_count = objects_in_sphere(1u, object_mask_biped, &machine->object.location,
                &machine->object.bounding_sphere_center, activation_radius, nearby_objects, 16);
        if ( nearby_count > 0 )
        {
            for ( __int16 i = 0; i < nearby_count; i = (__int16)(i + 1) )
            {
                object_datum *found_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, nearby_objects[i])->datum;
                unsigned __int8 counts_as_blocking = 1;
                /* unit_definition.unit (_unit_definition) @ +380, .flags @ +0 — dword index 95 in the raw form */
                if ( (found_object->object.damage_flags & (1u << _object_dead_bit)) != 0
                  || (TAG_GET(unit_definition, found_object->definition_index)->unit.flags
                        & (1u << _unit_definition_cannot_open_doors_automatically_bit)) != 0 )
                {
                    counts_as_blocking = 0;
                }
                /* one-way, fully-closed door ignores friendly units standing in front of it */
                if ( (machine->machine.flags & (1u << _machine_one_sided_bit)) != 0
                  && machine->device.position == 0.0f
                  && !game_team_is_enemy(1, found_object->object.owner_team_index)
                  && (((found_object->object.bounding_sphere_center.n[0] - machine->object.bounding_sphere_center.n[0])
                                  * machine->object.forward.n[0])
                          + ((machine->object.forward.n[2]
                                          * (found_object->object.bounding_sphere_center.n[2] - machine->object.bounding_sphere_center.n[2]))
                                  + (machine->object.forward.n[1]
                                          * (found_object->object.bounding_sphere_center.n[1] - machine->object.bounding_sphere_center.n[1])))) > 0.0f )
                {
                    counts_as_blocking = 0;
                }
                if ( counts_as_blocking )
                    blocked = 1;
            }
        }

        if ( blocked )
        {
            unsigned __int16 group_index = (unsigned __int16)machine->device.position_group_index;
            if ( group_index != 0xFFFF )
                device_group_set_desired_value(group_index, 1.0f);
            machine->machine.door_open_ticks = -3;
        }
    }

    /* --- stage 3: auto-close after the open delay --- */
    if ( definition->machine.type == _machine_door )
    {
        if ( machine->device.position == 1.0f )
        {
            int open_timer = machine->machine.door_open_ticks + 1;
            machine->machine.door_open_ticks = open_timer;
            if ( open_timer > definition->machine.runtime_door_open_ticks )
            {
                unsigned __int16 group_index = (unsigned __int16)machine->device.position_group_index;
                if ( group_index != 0xFFFF )
                    device_group_set_desired_value(group_index, 0.0f);
            }
        }
        else
        {
            machine->machine.door_open_ticks = 0;
        }
    }

    /* --- stage 4: lift/elevator machines carry their riders by the drive node's per-tick delta --- */
    /* definition->machine.flags is the tag flags enum (machine_flags), not the runtime
     * machine_datum_flags; bit 2 there is the elevator flag (both enums share bit index 2). */
    if ( (definition->machine.flags & (1u << _machine_is_elevator_bit)) != 0 )
    {
        unsigned __int16 node_index = (unsigned __int16)definition->machine.elevator_node_index;
        if ( node_index != 0xFFFF )
        {
            real_matrix4x3 *node_matrix = object_get_node_matrix(machine_index, node_index);
            float delta_x = (node_matrix->n[3][0] - machine->machine.elevator_position.n[0]);
            float delta_y = (node_matrix->n[3][1] - machine->machine.elevator_position.n[1]);
            float delta_z = (node_matrix->n[3][2] - machine->machine.elevator_position.n[2]);
            if ( delta_x != 0.0f || delta_y != 0.0f || delta_z != 0.0f )
            {
                int rider_objects[2048];
                __int16 rider_count = objects_in_sphere(1u, object_mask_biped, &machine->object.location,
                        &machine->object.bounding_sphere_center, machine->object.bounding_sphere_radius, rider_objects, 2048);
                if ( rider_count > 0 )
                {
                    for ( __int16 i = 0; i < rider_count; i = (__int16)(i + 1) )
                    {
                        int rider_index = rider_objects[i];
                        biped_datum *rider = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, rider_index)->datum;
                        if ( rider->biped.elevator_object_index == machine_index )
                        {
                            real_point3d rider_position;
                            rider_position.n[0] = rider->object.position.n[0] + delta_x;
                            rider_position.n[1] = rider->object.position.n[1] + delta_y;
                            rider_position.n[2] = rider->object.position.n[2] + delta_z;
                            object_translate(rider_index, &rider_position, 0);
                        }
                    }
                }
            }
            machine->machine.elevator_position.n[0] = node_matrix->n[3][0];
            machine->machine.elevator_position.n[1] = node_matrix->n[3][1];
            machine->machine.elevator_position.n[2] = node_matrix->n[3][2];
        }
    }

    /* --- reposition the object if any stage flagged it --- */
    if ( (machine->device.flags & (1u << _device_animation_changed_bit)) != 0 )
    {
        object_translate(machine_index, &machine->object.position, 0);
        machine->device.flags &= ~(1u << _device_animation_changed_bit);
    }
    return 1;
}
