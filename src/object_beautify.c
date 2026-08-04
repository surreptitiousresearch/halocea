/* object_beautify @0x836ECBE8 — set or clear an object's "beautiful" rendering flag (object flags +0x10 bit
 * 0x400000), used to force highest-detail rendering for cinematic objects. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


void object_beautify(int object_index, uint8_t beautiful)
{
    if ( object_index == -1 )
        return;
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    if ( beautiful )
        object_data->object.flags |= (1u << _object_movie_star_bit);
    else
        object_data->object.flags &= ~(1u << _object_movie_star_bit);
}
