/* item_new @0x83757BD0 */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/item_datum.h"
#include "headers/object_header_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

uint8_t item_new(int item_index)
{
    item_datum *object = (item_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, item_index)->datum;

    /* 0x6000 = force dynamic+static lighting recompute on spawn */
    object->object.flags |= (1u << _object_dynamic_lighting_recompute_bit) | (1u << _object_static_lighting_recompute_bit);
    object->item.last_owned_time = game_time_get();
    object->item.ignore_object_index = -1;
    return 1;
}
