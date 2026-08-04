/* unit_drop_item @0x836CC260 — detach an item the unit is holding and toss it free into the world.
 *
 * If the item has no parent (object.parent_object_index == -1) it is first re-shown and re-parented to the unit's
 * left hand. The item is then removed from the inventory and detached, its velocities are zeroed, and it is
 * given a randomized toss: a velocity in a narrow cone around the unit's facing (+/- ~22.5 deg) at a small
 * random speed, plus the unit's own velocity so it carries forward. Finally the item is forced back inside
 * the BSP — if that fails (and we're not in a game-engine match) the item is deleted, and if the unit is
 * flagged for cleanup (unit.flags & 0x100000) the item is deleted regardless. */

#include <stdint.h>
#include "headers/item_datum.h"
#include "headers/unit_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"

#include "headers/location.h"
extern const char empty_string[];     /* empty marker-name string */

extern void object_reconnect_to_map(int object_index, const location *location_in);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern void object_attach_to_marker(int parent_object_index, const char *parent_marker_name, int child_object_index, const char *child_marker_name);
extern void item_in_unit_inventory(int item_index, int owner_unit_index);
extern void object_detach(int child_object_index);
extern uint32_t *get_global_random_seed_address(void);
/* DEVIATION: the decompiler placed the `out` buffer in r5; disasm shows it is r7 — the two float cone
 * angles skip their GPR slots (PPC EABI), and `out` is the same stack vector later scaled below. */
extern real_vector3d *seed_random_vector_in_cone3d(uint32_t *seed, const real_vector3d *axis, float inner_cone_angle, float outer_cone_angle, real_vector3d *result);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern void item_accelerate(int item_index, const real_vector3d *acceleration, uint8_t detonates_explosives);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern uint8_t object_force_inside_bsp(int object_index, int ignore_object_index, const real_point3d *known_good_point);
extern uint8_t game_engine_running(void);
extern void object_delete(int object_index);

void unit_drop_item(int unit_index, int item_index)
{
    item_datum *item_data = ((item_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, item_index)->datum);
    unit_datum *unit_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    if ( item_data->object.parent_object_index == -1 )
    {
        object_reconnect_to_map(item_index, NULL);
        object_set_visibility(item_index, 1u);
        object_attach_to_marker(unit_index, "left hand", item_index, empty_string);
    }

    item_in_unit_inventory(item_index, -1);
    object_detach(item_index);
    item_data->object.translational_velocity = *global_zero_vector3d;
    item_data->object.angular_velocity = *global_zero_vector3d;

    /* randomized toss direction in a ~45-degree cone around the unit's aiming vector */
    real_vector3d toss_velocity;
    seed_random_vector_in_cone3d(get_global_random_seed_address(),
                                 &unit_data->unit.aiming_vector, 0.0f, 0.39269909f, &toss_velocity);
    float toss_speed = real_seed_random_range(get_global_random_seed_address(), 0.026666667f, 0.040000003f);
    toss_velocity.n[0] *= toss_speed;
    toss_velocity.n[1] *= toss_speed;
    toss_velocity.n[2] *= toss_speed;

    real_vector3d unit_velocity;
    object_get_velocities(unit_index, &unit_velocity, NULL);
    toss_velocity.n[0] += unit_velocity.n[0];
    toss_velocity.n[1] += unit_velocity.n[1];
    toss_velocity.n[2] += unit_velocity.n[2];

    item_data->item.ignore_object_index = unit_index;  /* ignore the tossing unit for collision */
    item_accelerate(item_index, &toss_velocity, 0);

    real_point3d camera_position;
    unit_get_camera_position(unit_index, &camera_position);
    if ( !object_force_inside_bsp(item_index, -1, &camera_position) && !game_engine_running() )
        object_delete(item_index);

    if ( (unit_data->unit.flags & (1u << _unit_doesnt_drop_items_bit)) != 0 )
        object_delete(item_index);
}
