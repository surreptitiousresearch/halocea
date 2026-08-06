/* message_game_settings_update_definition @ 0x841819B8 (.data, 232 bytes) — the message-delta message definition for
 * `_message_game_settings_update` (id 32): the game settings (game name, map, variant and player list) have changed.
 * The message carries no per-message header (header_fields is the shared empty NO_HEADER set);
 * 11 body fields.
 *
 * Per-field provenance, decoded big-endian straight from the image bytes:
 *   +0x00 definition_type                    = 0x00000020 -> _message_game_settings_update
 *   +0x04 header_size_in_bits                = 0xFFFFFFFF -> -1
 *   +0x08 iteration_size_in_bits             = 0xFFFFFFFF -> -1
 *   +0x0C iteration_independent_size_in_bits = 0xFFFFFFFF -> -1
 *   +0x10 maximum_total_size                 = 0xFFFFFFFF -> -1
 *   +0x14 max_iterations                     = 0x00000001 -> 1
 *   +0x18 initialized                        = 0x00 (+0x19..0x1B pad = 00 00 00)
 *   +0x1C header_fields                      = 0x841845B8 -> &message_header_NO_HEADER_definition
 *   +0x20 body_fields.count                  = 0x0000000B -> 11
 *   +0x24 body_fields.maximum_data_size      = 0xFFFFFFFF -> -1
 *   +0x28   field_references[ 0]             = { 0x841818E8 -> &field_properties_network_game_name_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x38   field_references[ 1]             = { 0x84181728 -> &field_properties_network_map_definition, offset 32, baseline_offset 32, initialized 0 }
 *   +0x48   field_references[ 2]             = { 0x8417D878 -> &field_properties_game_variant_definition, offset 164, baseline_offset 164, initialized 0 }
 *   +0x58   field_references[ 3]             = { 0x841832D0 -> &field_properties_integer_small_definition, offset 316, baseline_offset 316, initialized 0 }
 *   +0x68   field_references[ 4]             = { 0x841832D0 -> &field_properties_integer_small_definition, offset 317, baseline_offset 317, initialized 0 }
 *   +0x78   field_references[ 5]             = { 0x84183338 -> &field_properties_integer_medium_definition, offset 318, baseline_offset 318, initialized 0 }
 *   +0x88   field_references[ 6]             = { 0x84183338 -> &field_properties_integer_medium_definition, offset 320, baseline_offset 320, initialized 0 }
 *   +0x98   field_references[ 7]             = { 0x84181950 -> &field_properties_network_game_players_definition, offset 322, baseline_offset 322, initialized 0 }
 *   +0xA8   field_references[ 8]             = { 0x841833D0 -> &field_properties_integer_large_definition, offset 1348, baseline_offset 1348, initialized 0 }
 *   +0xB8   field_references[ 9]             = { 0x841833D0 -> &field_properties_integer_large_definition, offset 1352, baseline_offset 1352, initialized 0 }
 *   +0xC8   field_references[10]             = { 0x841833D0 -> &field_properties_integer_large_definition, offset 1356, baseline_offset 1356, initialized 0 }
 *   +0xD8   field_references[11]             = { 0x00000000, 0, 0, 0 }  <- terminator
 *
 * Layout: DB types_members for _message_definition (40 bytes, 32-bit target) — definition_type,
 * header_size_in_bits, iteration_size_in_bits, iteration_independent_size_in_bits, maximum_total_size,
 * max_iterations, initialized(+3 pad), header_fields, then the inline body_fields set
 * (count, maximum_data_size, field_references[]).
 * Element type and count: body_fields.field_references[] is the struct's trailing flexible array,
 * so the object's real extent fixes its length. This symbol runs to the next .data name at
 * 0x84181AA0, i.e. 232 bytes, and 0x28 + (11 + 1) * 16 == 232 — 16 being the DB member stride of
 * _field_reference_definition. So the array holds body_fields.count live entries plus one all-zero
 * terminator; that identity holds for all 46 message definitions with no exceptions.
 * The four -1 words, `initialized` and body_fields.maximum_data_size are load-time placeholders:
 * message_definition_initialize @0x837A2A50 computes and stores every one of them
 * (stw -> +0x04/+0x08/+0x0C/+0x10/+0x24, stb 1 -> +0x18).
 */
#include "../headers/message_definition.h"
#include "../headers/message_delta_message_ids.h"

extern _message_definition_field_reference_set message_header_NO_HEADER_definition;                           /* 0x841845B8 */
extern _field_properties_definition field_properties_network_game_name_definition;                                 /* 0x841818E8 */
extern _field_properties_definition field_properties_network_map_definition;                                       /* 0x84181728 */
extern _field_properties_definition field_properties_game_variant_definition;                                      /* 0x8417D878 */
extern _field_properties_definition field_properties_integer_small_definition;                                     /* 0x841832D0 */
extern _field_properties_definition field_properties_integer_medium_definition;                                    /* 0x84183338 */
extern _field_properties_definition field_properties_network_game_players_definition;                              /* 0x84181950 */
extern _field_properties_definition field_properties_integer_large_definition;                                     /* 0x841833D0 */

_message_definition message_game_settings_update_definition =
{
    _message_game_settings_update,           /* definition_type */
    -1,                                      /* header_size_in_bits */
    -1,                                      /* iteration_size_in_bits */
    -1,                                      /* iteration_independent_size_in_bits */
    -1,                                      /* maximum_total_size */
    1,                                       /* max_iterations */
    0,                                       /* initialized */
    { 0, 0, 0 },                             /* _pad0 */
    &message_header_NO_HEADER_definition,    /* header_fields */
    {                                        /* body_fields */
        11,                                  /* count */
        -1,                                  /* maximum_data_size */
        {   /* field_references[count + terminator]: { properties, offset, baseline_offset, initialized } */
            { &field_properties_network_game_name_definition, 0, 0, 0 },         /* [ 0] */
            { &field_properties_network_map_definition, 32, 32, 0 },             /* [ 1] */
            { &field_properties_game_variant_definition, 164, 164, 0 },          /* [ 2] */
            { &field_properties_integer_small_definition, 316, 316, 0 },         /* [ 3] */
            { &field_properties_integer_small_definition, 317, 317, 0 },         /* [ 4] */
            { &field_properties_integer_medium_definition, 318, 318, 0 },        /* [ 5] */
            { &field_properties_integer_medium_definition, 320, 320, 0 },        /* [ 6] */
            { &field_properties_network_game_players_definition, 322, 322, 0 },  /* [ 7] */
            { &field_properties_integer_large_definition, 1348, 1348, 0 },       /* [ 8] */
            { &field_properties_integer_large_definition, 1352, 1352, 0 },       /* [ 9] */
            { &field_properties_integer_large_definition, 1356, 1356, 0 },       /* [10] */
            { 0, 0, 0, 0 },                                                      /* [11] terminator */
        },
    },
};
