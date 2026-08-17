/* sound_scenery_new @0x837E9CF8 */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


uint8_t sound_scenery_new(int object_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object->object.flags |= (1u << _object_shadowless_bit);
    return 1;
}
