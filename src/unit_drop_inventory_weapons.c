/* unit_drop_inventory_weapons @0x836CC7A8 — eject every weapon in a unit's inventory except the one it is
 * currently holding (e.g. on death). The unit carries up to four weapon_object_indices; for each
 * occupied slot that isn't the equipped slot, the weapon is briefly re-attached to the left hand to inherit a
 * spawn pose, removed from the inventory, given a random scatter velocity inside a ~22.5-degree cone around
 * the unit's aim plus the unit's own velocity, accelerated, and the slot cleared. Weapons that can't be
 * forced inside the BSP (outside a running game engine), whose owner is flagged 0x100000, or that can't be
 * fired (on the host/local side) are deleted instead of left in the world.
 *
 * DEVIATION: seed_random_vector_in_cone3d's two float cone-angle args shadow GPR slots (FPR-shadow), so the
 * decompiler lost its result pointer (uninitialized v11); it is the scatter vector, restored from disasm. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/object_placement_data.h"
#include "headers/location.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/game_connection.h"
#include "headers/unit_flags.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"


#include "headers/location.h"
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
extern uint8_t weapon_can_be_fired(int weapon_index);
extern int16_t game_connection(void);

void unit_drop_inventory_weapons(int unit_index)
{
    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    for (int slot = 0; slot < MAXIMUM_WEAPONS_PER_UNIT; slot = (__int16)(slot + 1))
    {
        int *weapon_slot = &unit_object->unit.weapon_object_indices[slot];
        int weapon = *weapon_slot;

        if (weapon == -1 || slot == unit_object->unit.current_weapon_index) /* skip empty/equipped slot */
            continue;

        weapon_datum *weapon_object = ((weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon)->datum);
        unit_datum *owner_object = unit_object;   /* decompiler refetched the same header slot */

        if (weapon_object->object.parent_object_index == -1) /* not parented */
        {
            object_reconnect_to_map(weapon, nullptr);
            object_set_visibility(weapon, 1);
            object_attach_to_marker(unit_index, "left hand", weapon, "");
        }
        item_in_unit_inventory(weapon, -1);
        object_detach(weapon);
        weapon_object->object.translational_velocity = *global_zero_vector3d;
        weapon_object->object.angular_velocity = *global_zero_vector3d;

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

        weapon_object->item.ignore_object_index = unit_index;
        item_accelerate(weapon, &scatter, 0);

        real_point3d camera_position;
        unit_get_camera_position(unit_index, &camera_position);
        if (!object_force_inside_bsp(weapon, -1, &camera_position) && !game_engine_running())
            object_delete(weapon);
        if ((owner_object->unit.flags & (1u << _unit_doesnt_drop_items_bit)) != 0)
            object_delete(weapon);

        if (slot == unit_object->unit.desired_weapon_index)
            unit_object->unit.desired_weapon_index = unit_object->unit.current_weapon_index;
        *weapon_slot = -1;

        if (!weapon_can_be_fired(weapon) && (!game_connection() || game_connection() == _game_connection_film_playback))
            object_delete(weapon);
    }
}
