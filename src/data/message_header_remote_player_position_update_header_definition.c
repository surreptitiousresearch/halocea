/* message_header_remote_player_position_update_header_definition @ 0x8417E0E0 (.data, 72 bytes) — the shared per-message HEADER
 * field-reference set "remote_player_position_update_header": the fields every message carrying this header serializes ahead
 * of its body. Referenced through _message_definition::header_fields by the message definitions
 * in src/data/message_*_definition.c.
 * DB applied_types: _message_definition_field_reference_set message_header_remote_player_position_update_header_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x000 count             = 0x00000003 -> 3
 *   +0x004 maximum_data_size = 0xFFFFFFFF -> -1
 *   +0x008   [ 0] = { 0x84183AC8 -> &field_properties_player_index_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x018   [ 1] = { 0x84183268 -> &field_properties_update_id_definition, offset 4, baseline_offset 4, initialized 0 }
 *   +0x028   [ 2] = { 0x84183120 -> &field_properties_remote_player_update_sequence_number_definition, offset 5, baseline_offset 5, initialized 0 }
 *   +0x038   [ 3] = { 0x00000000, 0, 0, 0 }   <- terminator
 * Extent: field_references[] is the struct's trailing flexible array, so the object's extent
 * fixes its length. This symbol is 72 bytes and 0x8 + (3 + 1) * 16 == 72 — 16 being the DB member
 * stride of _field_reference_definition — so the array holds `count` live entries plus one all-zero
 * terminator. Confirmed per object for all 8 header sets, no exceptions. The terminator is load-
 * bearing, not decoration: message_definition_field_reference_set_initialize @0x837A2568 returns
 * false unless the entry immediately after the last live one has a null properties pointer and
 * zero offsets.
 * maximum_data_size is a load-time placeholder computed by
 * message_definition_field_reference_set_calculate_maximum_size @0x837A0A58.
 */

#include "../headers/message_definition_field_reference_set.h"
#include "../headers/field_properties_definition.h"

extern _field_properties_definition field_properties_player_index_definition; /* 0x84183AC8 */
extern _field_properties_definition field_properties_remote_player_update_sequence_number_definition; /* 0x84183120 */
extern _field_properties_definition field_properties_update_id_definition; /* 0x84183268 */

_message_definition_field_reference_set message_header_remote_player_position_update_header_definition =
{
    3,                                       /* count */
    -1,                                      /* maximum_data_size */
    {   /* field_references[count + terminator]: { properties, offset, baseline_offset, initialized } */
        { &field_properties_player_index_definition, 0, 0, 0 },  /* [ 0] */
        { &field_properties_update_id_definition, 4, 4, 0 },  /* [ 1] */
        { &field_properties_remote_player_update_sequence_number_definition, 5, 5, 0 },  /* [ 2] */
        { 0, 0, 0, 0 },  /* [ 3] terminator */
    },
};
