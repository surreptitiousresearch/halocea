/* message_player_create_definition @ 0x84175A70 (.data, 120 bytes) — the message-delta message definition for
 * `_message_player_create` (id 7): a player has joined the game.
 * The message carries no per-message header (header_fields is the shared empty NO_HEADER set);
 * 4 body fields.
 *
 * Per-field provenance, decoded big-endian straight from the image bytes:
 *   +0x00 definition_type                    = 0x00000007 -> _message_player_create
 *   +0x04 header_size_in_bits                = 0xFFFFFFFF -> -1
 *   +0x08 iteration_size_in_bits             = 0xFFFFFFFF -> -1
 *   +0x0C iteration_independent_size_in_bits = 0xFFFFFFFF -> -1
 *   +0x10 maximum_total_size                 = 0xFFFFFFFF -> -1
 *   +0x14 max_iterations                     = 0x00000001 -> 1
 *   +0x18 initialized                        = 0x00 (+0x19..0x1B pad = 00 00 00)
 *   +0x1C header_fields                      = 0x841845B8 -> &message_header_NO_HEADER_definition
 *   +0x20 body_fields.count                  = 0x00000004 -> 4
 *   +0x24 body_fields.maximum_data_size      = 0xFFFFFFFF -> -1
 *   +0x28   field_references[ 0]             = { 0x841832D0 -> &field_properties_integer_small_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x38   field_references[ 1]             = { 0x841833D0 -> &field_properties_integer_large_definition, offset 4, baseline_offset 4, initialized 0 }
 *   +0x48   field_references[ 2]             = { 0x84183AC8 -> &field_properties_player_index_definition, offset 8, baseline_offset 8, initialized 0 }
 *   +0x58   field_references[ 3]             = { 0x841833D0 -> &field_properties_integer_large_definition, offset 12, baseline_offset 12, initialized 0 }
 *   +0x68   field_references[ 4]             = { 0x00000000, 0, 0, 0 }  <- terminator
 *
 * Layout: DB types_members for _message_definition (40 bytes, 32-bit target) — definition_type,
 * header_size_in_bits, iteration_size_in_bits, iteration_independent_size_in_bits, maximum_total_size,
 * max_iterations, initialized(+3 pad), header_fields, then the inline body_fields set
 * (count, maximum_data_size, field_references[]).
 * Element type and count: body_fields.field_references[] is the struct's trailing flexible array,
 * so the object's real extent fixes its length. This symbol runs to the next .data name at
 * 0x84175AE8, i.e. 120 bytes, and 0x28 + (4 + 1) * 16 == 120 — 16 being the DB member stride of
 * _field_reference_definition. So the array holds body_fields.count live entries plus one all-zero
 * terminator; that identity holds for all 46 message definitions with no exceptions.
 * The four -1 words, `initialized` and body_fields.maximum_data_size are load-time placeholders:
 * message_definition_initialize @0x837A2A50 computes and stores every one of them
 * (stw -> +0x04/+0x08/+0x0C/+0x10/+0x24, stb 1 -> +0x18).
 */
#include "../headers/message_definition.h"
#include "../headers/message_delta_message_ids.h"

extern _message_definition_field_reference_set message_header_NO_HEADER_definition;                           /* 0x841845B8 */
extern _field_properties_definition field_properties_integer_small_definition;                                     /* 0x841832D0 */
extern _field_properties_definition field_properties_integer_large_definition;                                     /* 0x841833D0 */
extern _field_properties_definition field_properties_player_index_definition;                                      /* 0x84183AC8 */

_message_definition message_player_create_definition =
{
    _message_player_create,                  /* definition_type */
    -1,                                      /* header_size_in_bits */
    -1,                                      /* iteration_size_in_bits */
    -1,                                      /* iteration_independent_size_in_bits */
    -1,                                      /* maximum_total_size */
    1,                                       /* max_iterations */
    0,                                       /* initialized */
    { 0, 0, 0 },                             /* _pad0 */
    &message_header_NO_HEADER_definition,    /* header_fields */
    {                                        /* body_fields */
        4,                                   /* count */
        -1,                                  /* maximum_data_size */
        {   /* field_references[count + terminator]: { properties, offset, baseline_offset, initialized } */
            { &field_properties_integer_small_definition, 0, 0, 0 },    /* [ 0] */
            { &field_properties_integer_large_definition, 4, 4, 0 },    /* [ 1] */
            { &field_properties_player_index_definition, 8, 8, 0 },     /* [ 2] */
            { &field_properties_integer_large_definition, 12, 12, 0 },  /* [ 3] */
            { 0, 0, 0, 0 },                                             /* [ 4] terminator */
        },
    },
};
