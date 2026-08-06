/* message_header_game_object_header_timestamped_definition @ 0x84184650 (.data, 104 bytes) — the shared per-message HEADER
 * field-reference set "game_object_header_timestamped": the fields every message carrying this header serializes ahead
 * of its body. Referenced through _message_definition::header_fields by the message definitions
 * in src/data/message_*_definition.c.
 * DB applied_types: _message_definition_field_reference_set message_header_game_object_header_timestamped_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x000 count             = 0x00000005 -> 5
 *   +0x004 maximum_data_size = 0xFFFFFFFF -> -1
 *   +0x008   [ 0] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 5, baseline_offset 5, initialized 0 }
 *   +0x018   [ 1] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 4, baseline_offset 4, initialized 0 }
 *   +0x028   [ 2] = { 0x84183A30 -> &field_properties_object_index_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x038   [ 3] = { 0x84183DD8 -> &field_properties_time_definition, offset 8, baseline_offset 8, initialized 0 }
 *   +0x048   [ 4] = { 0x84183960 -> &field_properties_boolean_definition, offset 6, baseline_offset 6, initialized 0 }
 *   +0x058   [ 5] = { 0x00000000, 0, 0, 0 }   <- terminator
 * Extent: field_references[] is the struct's trailing flexible array, so the object's extent
 * fixes its length. This symbol is 104 bytes and 0x8 + (5 + 1) * 16 == 104 — 16 being the DB member
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

extern _field_properties_definition field_properties_boolean_definition;   /* 0x84183960 */
extern _field_properties_definition field_properties_integer_small_definition; /* 0x841832D0 */
extern _field_properties_definition field_properties_object_index_definition; /* 0x84183A30 */
extern _field_properties_definition field_properties_time_definition;      /* 0x84183DD8 */

_message_definition_field_reference_set message_header_game_object_header_timestamped_definition =
{
    5,                                       /* count */
    -1,                                      /* maximum_data_size */
    {   /* field_references[count + terminator]: { properties, offset, baseline_offset, initialized } */
        { &field_properties_integer_small_definition, 5, 5, 0 },  /* [ 0] */
        { &field_properties_integer_small_definition, 4, 4, 0 },  /* [ 1] */
        { &field_properties_object_index_definition, 0, 0, 0 },  /* [ 2] */
        { &field_properties_time_definition, 8, 8, 0 },  /* [ 3] */
        { &field_properties_boolean_definition, 6, 6, 0 },  /* [ 4] */
        { 0, 0, 0, 0 },  /* [ 5] terminator */
    },
};
