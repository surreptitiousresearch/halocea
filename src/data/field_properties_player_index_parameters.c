/* field_properties_player_index_parameters @ 0x84183A98 (.data, 44 bytes)
 * DB applied_types: _field_type_translated_index_parameters field_properties_player_index_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 maximum_active_at_once     = 0x00000021
 *   +0x04 initial_translation_table_size = 0x00000021
 *   +0x08 number_of_bits             = 0x00000000
 *   +0x0C translation_table          = 0x000000000000000000000000000000000000000000000000
 *   +0x24 translated_index_allocation_cursor = 0x00000000
 *   +0x28 translated_index_allocations = 0x00000000
 * second-order .data; _field_type_translated_index_parameters reconstructed from binary bytes.
 */
#include "../headers/_field_type_translated_index_parameters.h"

_field_type_translated_index_parameters field_properties_player_index_parameters = {
    .maximum_active_at_once         = 33, /* 0x00000021 */
    .initial_translation_table_size = 33, /* 0x00000021 */
    /* number_of_bits, translation_table, allocation_cursor, translated_index_allocations all zero */
};
