/* message_unit_kill_definition @ 0x84176000 (.data, 248 bytes) — the message-delta message definition for
 * `_message_unit_kill` (id 12): a unit has been killed, with the manner of the kill.
 * The message carries no per-message header (header_fields is the shared empty NO_HEADER set);
 * 12 body fields.
 *
 * Per-field provenance, decoded big-endian straight from the image bytes:
 *   +0x00 definition_type                    = 0x0000000C -> _message_unit_kill
 *   +0x04 header_size_in_bits                = 0xFFFFFFFF -> -1
 *   +0x08 iteration_size_in_bits             = 0xFFFFFFFF -> -1
 *   +0x0C iteration_independent_size_in_bits = 0xFFFFFFFF -> -1
 *   +0x10 maximum_total_size                 = 0xFFFFFFFF -> -1
 *   +0x14 max_iterations                     = 0x00000001 -> 1
 *   +0x18 initialized                        = 0x00 (+0x19..0x1B pad = 00 00 00)
 *   +0x1C header_fields                      = 0x841845B8 -> &message_header_NO_HEADER_definition
 *   +0x20 body_fields.count                  = 0x0000000C -> 12
 *   +0x24 body_fields.maximum_data_size      = 0xFFFFFFFF -> -1
 *   +0x28   field_references[ 0]             = { 0x84183A30 -> &field_properties_object_index_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x38   field_references[ 1]             = { 0x84183960 -> &field_properties_boolean_definition, offset 4, baseline_offset 4, initialized 0 }
 *   +0x48   field_references[ 2]             = { 0x84183960 -> &field_properties_boolean_definition, offset 5, baseline_offset 5, initialized 0 }
 *   +0x58   field_references[ 3]             = { 0x84183960 -> &field_properties_boolean_definition, offset 6, baseline_offset 6, initialized 0 }
 *   +0x68   field_references[ 4]             = { 0x84183960 -> &field_properties_boolean_definition, offset 7, baseline_offset 7, initialized 0 }
 *   +0x78   field_references[ 5]             = { 0x84183960 -> &field_properties_boolean_definition, offset 8, baseline_offset 8, initialized 0 }
 *   +0x88   field_references[ 6]             = { 0x84183960 -> &field_properties_boolean_definition, offset 9, baseline_offset 9, initialized 0 }
 *   +0x98   field_references[ 7]             = { 0x84183960 -> &field_properties_boolean_definition, offset 10, baseline_offset 10, initialized 0 }
 *   +0xA8   field_references[ 8]             = { 0x84183338 -> &field_properties_integer_medium_definition, offset 12, baseline_offset 12, initialized 0 }
 *   +0xB8   field_references[ 9]             = { 0x84183438 -> &field_properties_real_definition, offset 16, baseline_offset 16, initialized 0 }
 *   +0xC8   field_references[10]             = { 0x84183C78 -> &field_properties_vector2d_definition, offset 20, baseline_offset 20, initialized 0 }
 *   +0xD8   field_references[11]             = { 0x841833D0 -> &field_properties_integer_large_definition, offset 28, baseline_offset 28, initialized 0 }
 *   +0xE8   field_references[12]             = { 0x00000000, 0, 0, 0 }  <- terminator
 *
 * Layout: DB types_members for _message_definition (40 bytes, 32-bit target) — definition_type,
 * header_size_in_bits, iteration_size_in_bits, iteration_independent_size_in_bits, maximum_total_size,
 * max_iterations, initialized(+3 pad), header_fields, then the inline body_fields set
 * (count, maximum_data_size, field_references[]).
 * Element type and count: body_fields.field_references[] is the struct's trailing flexible array,
 * so the object's real extent fixes its length. This symbol runs to the next .data name at
 * 0x841760F8, i.e. 248 bytes, and 0x28 + (12 + 1) * 16 == 248 — 16 being the DB member stride of
 * _field_reference_definition. So the array holds body_fields.count live entries plus one all-zero
 * terminator; that identity holds for all 46 message definitions with no exceptions.
 * The four -1 words, `initialized` and body_fields.maximum_data_size are load-time placeholders:
 * message_definition_initialize @0x837A2A50 computes and stores every one of them
 * (stw -> +0x04/+0x08/+0x0C/+0x10/+0x24, stb 1 -> +0x18).
 */
#include "../headers/message_definition.h"
#include "../headers/message_delta_message_ids.h"

extern _message_definition_field_reference_set message_header_NO_HEADER_definition;                           /* 0x841845B8 */
extern _field_properties_definition field_properties_object_index_definition;                                      /* 0x84183A30 */
extern _field_properties_definition field_properties_boolean_definition;                                           /* 0x84183960 */
extern _field_properties_definition field_properties_integer_medium_definition;                                    /* 0x84183338 */
extern _field_properties_definition field_properties_real_definition;                                              /* 0x84183438 */
extern _field_properties_definition field_properties_vector2d_definition;                                          /* 0x84183C78 */
extern _field_properties_definition field_properties_integer_large_definition;                                     /* 0x841833D0 */

_message_definition message_unit_kill_definition =
{
    _message_unit_kill,                      /* definition_type */
    -1,                                      /* header_size_in_bits */
    -1,                                      /* iteration_size_in_bits */
    -1,                                      /* iteration_independent_size_in_bits */
    -1,                                      /* maximum_total_size */
    1,                                       /* max_iterations */
    0,                                       /* initialized */
    { 0, 0, 0 },                             /* _pad0 */
    &message_header_NO_HEADER_definition,    /* header_fields */
    {                                        /* body_fields */
        12,                                  /* count */
        -1,                                  /* maximum_data_size */
        {   /* field_references[count + terminator]: { properties, offset, baseline_offset, initialized } */
            { &field_properties_object_index_definition, 0, 0, 0 },      /* [ 0] */
            { &field_properties_boolean_definition, 4, 4, 0 },           /* [ 1] */
            { &field_properties_boolean_definition, 5, 5, 0 },           /* [ 2] */
            { &field_properties_boolean_definition, 6, 6, 0 },           /* [ 3] */
            { &field_properties_boolean_definition, 7, 7, 0 },           /* [ 4] */
            { &field_properties_boolean_definition, 8, 8, 0 },           /* [ 5] */
            { &field_properties_boolean_definition, 9, 9, 0 },           /* [ 6] */
            { &field_properties_boolean_definition, 10, 10, 0 },         /* [ 7] */
            { &field_properties_integer_medium_definition, 12, 12, 0 },  /* [ 8] */
            { &field_properties_real_definition, 16, 16, 0 },            /* [ 9] */
            { &field_properties_vector2d_definition, 20, 20, 0 },        /* [10] */
            { &field_properties_integer_large_definition, 28, 28, 0 },   /* [11] */
            { 0, 0, 0, 0 },                                              /* [12] terminator */
        },
    },
};
