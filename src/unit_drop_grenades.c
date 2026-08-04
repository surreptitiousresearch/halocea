/* unit_drop_grenades @0x836CC458 — eject all of a unit's grenades into the world (e.g. on death). For each
 * of the two grenade types, while the unit still carries one, a grenade item object is spawned from the game
 * globals' grenade definition, briefly attached to the unit's left hand to inherit a spawn pose, removed from
 * the inventory, given a random scatter velocity inside a ~22.5-degree cone around the unit's aim plus the
 * unit's own velocity, and accelerated. Grenades that can't be forced inside the BSP (outside a running game
 * engine) or whose owner is flagged 0x100000 are deleted instead of left in the world. The unit's per-type
 * grenade count is decremented as each is dropped.
 *
 * DEVIATION: seed_random_vector_in_cone3d's two float cone-angle args shadow GPR slots (FPR-shadow), so the
 * decompiler lost its trailing result pointer (shown as an uninitialized v13); it is the scatter vector,
 * restored from the disassembly. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/item_datum.h"
#include "headers/game_globals_tag.h"
#include "headers/game_globals_grenade.h"
#include "headers/object_placement_data.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/location.h"
#include "headers/unit_flags.h"
#include "headers/unit_grenade_type.h"
#include <stdint.h>
#include "headers/blam_data_globals.h"


#include "headers/location.h"
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern void object_disconnect_from_map(int object_index);
extern void object_reconnect_to_map(int object_index, const location *location_in);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern void object_attach_to_marker(int parent_object_index, const char *parent_marker_name, int child_object_index, const char *child_marker_name);
extern void item_in_unit_inventory(int item_index, int owner_unit_index);
extern void object_detach(int child_object_index);
extern uint32_t *get_global_random_seed_address(void);
extern real_vector3d *seed_random_vector_in_cone3d(uint32_t *seed, const real_vector3d *axis, float inner_cone_angle, float outer_cone_angle, real_vector3d *result);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern void item_accelerate(int item_index, const real_vector3d *acceleration, uint8_t detonates_explosives);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern uint8_t object_force_inside_bsp(int object_index, int ignore_object_index, const real_point3d *known_good_point);
extern uint8_t game_engine_running(void);
extern void object_delete(int object_index);

void unit_drop_grenades(int unit_index)
{
    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    for (int grenade_type = 0; grenade_type < NUMBER_OF_UNIT_GRENADE_TYPES; grenade_type = (__int16)(grenade_type + 1))
    {
        char *count_address = &unit_object->unit.grenade_counts[grenade_type];
        game_globals_grenade *grenade_definition =
            &((game_globals_grenade *)global_game_globals->grenades.address)[grenade_type];

        if (*count_address <= 0)
            continue;

        char remaining;
        do
        {
            object_placement_data placement;
            object_placement_data_new(&placement, grenade_definition->item.index, unit_index);
            int grenade = object_new(&placement);
            if (grenade != -1)
            {
                object_disconnect_from_map(grenade);
                item_datum *grenade_object =
                    (item_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, grenade)->datum;
                unit_datum *owner_object =
                    (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

                if (grenade_object->object.parent_object_index == -1) /* not parented */
                {
                    object_reconnect_to_map(grenade, nullptr);
                    object_set_visibility(grenade, 1);
                    object_attach_to_marker(unit_index, "left hand", grenade, "");
                }
                item_in_unit_inventory(grenade, -1);
                object_detach(grenade);
                grenade_object->object.translational_velocity = *global_zero_vector3d;
                grenade_object->object.angular_velocity = *global_zero_vector3d;

                real_vector3d scatter;
                seed_random_vector_in_cone3d(get_global_random_seed_address(),
                                             &owner_object->unit.aiming_vector,
                                             0.0f, 0.39269909f, &scatter);
                float speed = real_seed_random_range(get_global_random_seed_address(), 0.026666667f, 0.040000003f);
                scatter.n[0] *= speed;
                scatter.n[1] *= speed;
                scatter.n[2] *= speed;

                real_vector3d unit_velocity;
                object_get_velocities(unit_index, &unit_velocity, nullptr);
                scatter.n[0] += unit_velocity.n[0];
                scatter.n[1] += unit_velocity.n[1];
                scatter.n[2] += unit_velocity.n[2];

                grenade_object->item.ignore_object_index = unit_index;
                item_accelerate(grenade, &scatter, 0);

                real_point3d camera_position;
                unit_get_camera_position(unit_index, &camera_position);
                if (!object_force_inside_bsp(grenade, -1, &camera_position) && !game_engine_running())
                    object_delete(grenade);
                if ((owner_object->unit.flags & (1u << _unit_doesnt_drop_items_bit)) != 0)
                    object_delete(grenade);
            }

            remaining = *count_address;
            *count_address = remaining - 1;
        }
        while ((char)(remaining - 1) > 0);
    }
}
