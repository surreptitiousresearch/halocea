/* object_get_actual_body_vitality @0x836B2588 — absolute body health = body fraction (object data +0xE0) times
 * the maximum body vitality. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"

extern float object_get_maximum_body_vitality(int object_index, uint8_t ignore_difficulty);

float object_get_actual_body_vitality(int object_index, uint8_t ignore_difficulty)
{
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    float body_fraction = object_data->object.body_vitality;
    return object_get_maximum_body_vitality(object_index, ignore_difficulty) * body_fraction;
}
