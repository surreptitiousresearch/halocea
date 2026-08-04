/* object_get_actual_shield_vitality @0x836B25E0 — absolute shield health = shield fraction (object data +0xE4)
 * times the maximum shield vitality. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"

extern float object_get_maximum_shield_vitality(int object_index, uint8_t ignore_difficulty);

float object_get_actual_shield_vitality(int object_index, uint8_t ignore_difficulty)
{
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    float shield_fraction = object_data->object.shield_vitality;
    return object_get_maximum_shield_vitality(object_index, ignore_difficulty) * shield_fraction;
}
