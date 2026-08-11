/* player_examine_nearby_objects_client @0x836AD178 — client-side variant of player_examine_nearby_objects:
 * gathers nearby objects around the player's on-foot unit and dispatches each by type. Items go through
 * the client item-examine path; vehicles and devices share the common handlers. No-op if the player has
 * no unit or the unit is riding something.
 *
 * DEVIATION: the DB prototype is single-argument; the decompiler invented phantom a2..a5 and mis-mapped
 * the objects_in_sphere buffer/count. Radius is a float arg (skips a GPR slot), so the real call passes
 * the 112-byte stack buffer as object_indices and 16 as maximum_count. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern int16_t objects_in_sphere(unsigned int class_flags, unsigned int type_flags, const location *location, const real_point3d *center, float radius, int *object_indices, int16_t maximum_count);
extern void player_examine_nearby_vehicle(int player_index, int vehicle_index);
extern void player_examine_nearby_item_client(int player_index, int item_index);
extern void player_examine_nearby_device(int player_index, int device_index);

void player_examine_nearby_objects_client(int player_index)
{
    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    if (unit_index == -1)
        return;

    object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if (unit->object.parent_object_index != -1)
        return;

    int nearby_objects[28];
    int count = objects_in_sphere(0, object_mask_unit | object_mask_item | object_mask_control,   /* 0x11F */ &unit->object.location,
                                  &unit->object.bounding_sphere_center,
                                  unit->object.bounding_sphere_radius,
                                  nearby_objects, 16);
    if (count <= 0)
        return;

    for (int i = 0; i < count; i = (int16_t)(i + 1))
    {
        int object_index = nearby_objects[i];
        object_datum *object =
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        unsigned int type = (uint16_t)object->object.type;
        if (type > object_type_control || type == object_type_biped)
            continue;

        if (type == object_type_vehicle)
            player_examine_nearby_vehicle(player_index, object_index);
        else if (type == object_type_weapon || type == object_type_equipment)
            player_examine_nearby_item_client(player_index, object_index);
        /* DEVIATION: the compiler's unsigned range test (type - 6 >= 2) unfolded into the two
         * named exclusions it encodes; with the guards above only object_type_control remains */
        else if (type != object_type_garbage && type != object_type_projectile
              && type != object_type_scenery && type != object_type_machine)
            player_examine_nearby_device(player_index, object_index);
    }
}
