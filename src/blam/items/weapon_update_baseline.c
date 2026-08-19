/* weapon_update_baseline @0x836DA960 — snapshot a weapon object's network baseline: record position,
 * translational velocity, age and both magazine round-totals into the baseline network-data copy, advance
 * the baseline generation index, mark the baseline valid and reset the message index. */

#include <stdint.h>
#include "headers/weapon_datum.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void weapon_update_baseline(int object_index)
{
    weapon_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_weapon);

    if ( object )
    {
        int16_t magazine_0_rounds_total = object->weapon.magazines[0].rounds_total;
        int16_t magazine_1_rounds_total = object->weapon.magazines[1].rounds_total;
        uint8_t next_generation = object->weapon.baseline_index + 1;

        object->weapon.baseline.position = object->object.position;
        object->weapon.baseline.age = object->weapon.age;
        object->weapon.baseline.translational_velocity = object->object.translational_velocity;
        object->weapon.baseline_valid = 1;
        object->weapon.baseline_index = next_generation;
        object->weapon.message_index = 0;
        object->weapon.baseline.magazine_0_rounds_total = magazine_0_rounds_total;
        object->weapon.baseline.magazine_1_rounds_total = magazine_1_rounds_total;
    }
}
