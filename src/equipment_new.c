/* equipment_new @0x837B5D80 — on server/local games, clear the equipment's 3 usage-tracking bytes (+580..582)
 * and its "in-use" byte (+9). Same object_header_data 12-byte-datum idiom as garbage_new.c. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/equipment_datum.h"
#include "headers/blam_data_globals.h"

extern int16_t game_connection(void);

uint8_t equipment_new(int equipment_index)
{
    equipment_datum *object = (equipment_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, equipment_index)->datum;
    __int16 connection = game_connection();

    if ( connection == 1 || connection == 2 )
    {
        object->equipment.baseline_valid = 0;
        object->equipment.baseline_index = 0;
        object->equipment.message_index = 0;
        object->object.was_network_at_rest = 0;
    }
    return 1;
}
