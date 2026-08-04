/* object_type_is_update_valid @0x83702840 — an incremental network update is valid to apply if the object
 * hasn't yet been updated from network (_object_has_been_updated_from_network_bit, bit 27 = 0x8000000), the mode isn't incremental, the
 * baseline matches, and either the update is newer than the last-valid message or hasn't aged past the
 * window. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/object_header_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


uint8_t object_type_is_update_valid(int object_index, const message_delta_processor_mode mode, const int baseline_index_from_update, int message_index_from_update, const int latest_valid_baseline_index, int latest_valid_message_index, const int maximum_message_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    if ( (object->object.flags & (1u << _object_has_been_updated_from_network_bit)) != 0x8000000
      || mode != _message_delta_mode_incremental
      || (baseline_index_from_update == latest_valid_baseline_index
          && (message_index_from_update > latest_valid_message_index
              || message_index_from_update - latest_valid_message_index + maximum_message_index < 30)) )
    {
        return 1;
    }
    return 0;
}
