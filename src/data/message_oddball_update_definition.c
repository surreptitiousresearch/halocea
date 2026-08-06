/* message_oddball_update_definition @ 0x84185AB8 (.data, 104 bytes) — the message-delta message definition for
 * `_message_oddball_update` (id 18): the oddball game engine's score arrays.
 * The message carries no per-message header (header_fields is the shared empty NO_HEADER set);
 * 3 body fields.
 *
 * Per-field provenance, decoded big-endian straight from the image bytes:
 *   +0x00 definition_type                    = 0x00000012 -> _message_oddball_update
 *   +0x04 header_size_in_bits                = 0xFFFFFFFF -> -1
 *   +0x08 iteration_size_in_bits             = 0xFFFFFFFF -> -1
 *   +0x0C iteration_independent_size_in_bits = 0xFFFFFFFF -> -1
 *   +0x10 maximum_total_size                 = 0xFFFFFFFF -> -1
 *   +0x14 max_iterations                     = 0x00000001 -> 1
 *   +0x18 initialized                        = 0x00 (+0x19..0x1B pad = 00 00 00)
 *   +0x1C header_fields                      = 0x841845B8 -> &message_header_NO_HEADER_definition
 *   +0x20 body_fields.count                  = 0x00000003 -> 3
 *   +0x24 body_fields.maximum_data_size      = 0xFFFFFFFF -> -1
 *   +0x28   field_references[ 0]             = { 0x841859E8 -> &field_properties_oddball_score_array_definition, offset 68, baseline_offset 68, initialized 0 }
 *   +0x38   field_references[ 1]             = { 0x841859E8 -> &field_properties_oddball_score_array_definition, offset 4, baseline_offset 4, initialized 0 }
 *   +0x48   field_references[ 2]             = { 0x841859E8 -> &field_properties_oddball_score_array_definition, offset 196, baseline_offset 196, initialized 0 }
 *   +0x58   field_references[ 3]             = { 0x00000000, 0, 0, 0 }  <- terminator
 *
 * Layout: DB types_members for _message_definition (40 bytes, 32-bit target) — definition_type,
 * header_size_in_bits, iteration_size_in_bits, iteration_independent_size_in_bits, maximum_total_size,
 * max_iterations, initialized(+3 pad), header_fields, then the inline body_fields set
 * (count, maximum_data_size, field_references[]).
 * Element type and count: body_fields.field_references[] is the struct's trailing flexible array,
 * so the object's real extent fixes its length. This symbol runs to the next .data name at
 * 0x84185B20, i.e. 104 bytes, and 0x28 + (3 + 1) * 16 == 104 — 16 being the DB member stride of
 * _field_reference_definition. So the array holds body_fields.count live entries plus one all-zero
 * terminator; that identity holds for all 46 message definitions with no exceptions.
 * The four -1 words, `initialized` and body_fields.maximum_data_size are load-time placeholders:
 * message_definition_initialize @0x837A2A50 computes and stores every one of them
 * (stw -> +0x04/+0x08/+0x0C/+0x10/+0x24, stb 1 -> +0x18).
 */
#include "../headers/message_definition.h"
#include "../headers/message_delta_message_ids.h"

extern _message_definition_field_reference_set message_header_NO_HEADER_definition;                           /* 0x841845B8 */
extern _field_properties_definition field_properties_oddball_score_array_definition;                               /* 0x841859E8 */

_message_definition message_oddball_update_definition =
{
    _message_oddball_update,                 /* definition_type */
    -1,                                      /* header_size_in_bits */
    -1,                                      /* iteration_size_in_bits */
    -1,                                      /* iteration_independent_size_in_bits */
    -1,                                      /* maximum_total_size */
    1,                                       /* max_iterations */
    0,                                       /* initialized */
    { 0, 0, 0 },                             /* _pad0 */
    &message_header_NO_HEADER_definition,    /* header_fields */
    {                                        /* body_fields */
        3,                                   /* count */
        -1,                                  /* maximum_data_size */
        {   /* field_references[count + terminator]: { properties, offset, baseline_offset, initialized } */
            { &field_properties_oddball_score_array_definition, 68, 68, 0 },    /* [ 0] */
            { &field_properties_oddball_score_array_definition, 4, 4, 0 },      /* [ 1] */
            { &field_properties_oddball_score_array_definition, 196, 196, 0 },  /* [ 2] */
            { 0, 0, 0, 0 },                                                     /* [ 3] terminator */
        },
    },
};
