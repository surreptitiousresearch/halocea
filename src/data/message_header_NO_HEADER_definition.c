/* message_header_NO_HEADER_definition @ 0x841845B8 (.data, 24 bytes) — the shared per-message HEADER
 * field-reference set "NO_HEADER": the fields every message carrying this header serializes ahead
 * of its body. Referenced through _message_definition::header_fields by the message definitions
 * in src/data/message_*_definition.c.
 * DB applied_types: _message_definition_field_reference_set message_header_NO_HEADER_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x000 count             = 0x00000000 -> 0
 *   +0x004 maximum_data_size = 0xFFFFFFFF -> -1
 *   +0x008   [ 0] = { 0x00000000, 0, 0, 0 }   <- terminator
 * Extent: field_references[] is the struct's trailing flexible array, so the object's extent
 * fixes its length. This symbol is 24 bytes and 0x8 + (0 + 1) * 16 == 24 — 16 being the DB member
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


_message_definition_field_reference_set message_header_NO_HEADER_definition =
{
    0,                                       /* count */
    -1,                                      /* maximum_data_size */
    {   /* field_references[count + terminator]: { properties, offset, baseline_offset, initialized } */
        { 0, 0, 0, 0 },  /* [ 0] terminator */
    },
};
