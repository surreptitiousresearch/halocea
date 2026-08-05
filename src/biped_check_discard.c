/* biped_check_discard @0x837AE500 — outside multiplayer, deletes a biped that has fallen far below the map
 * (object.position.z < -2000) and is either flagged outside-of-map (_object_outside_of_map_bit)
 * or has no owning cluster (object.location.cluster_index == 0xFFFF). Always returns 0.
 *
 * Object header lookup matches the established salted object-header idiom (see vehicle_reset.c). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_engine_running(void);
extern void object_delete(int object_index);

uint8_t biped_check_discard(int biped_index)
{
    object_datum *biped = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;

    if ( !game_engine_running()
      && ((biped->object.flags & (1u << _object_outside_of_map_bit)) != 0
          || (uint16_t)biped->object.location.cluster_index == 0xFFFF)
      && biped->object.position.n[2] < -2000.0f )
    {
        object_delete(biped_index);
    }

    return 0;
}
