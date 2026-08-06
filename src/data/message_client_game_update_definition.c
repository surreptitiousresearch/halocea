/* message_client_game_update_definition @ 0x8417DD60 (.data, 200 bytes) — the message-delta message definition for
 * `_message_client_game_update` (id 13): the per-tick client-to-server control update.
 * Per-message header set: message_header_client_game_update_header_definition; 9 body fields.
 *
 * Per-field provenance, decoded big-endian straight from the image bytes:
 *   +0x00 definition_type                    = 0x0000000D -> _message_client_game_update
 *   +0x04 header_size_in_bits                = 0xFFFFFFFF -> -1
 *   +0x08 iteration_size_in_bits             = 0xFFFFFFFF -> -1
 *   +0x0C iteration_independent_size_in_bits = 0xFFFFFFFF -> -1
 *   +0x10 maximum_total_size                 = 0xFFFFFFFF -> -1
 *   +0x14 max_iterations                     = 0x00000001 -> 1
 *   +0x18 initialized                        = 0x00 (+0x19..0x1B pad = 00 00 00)
 *   +0x1C header_fields                      = 0x8417DD38 -> &message_header_client_game_update_header_definition
 *   +0x20 body_fields.count                  = 0x00000009 -> 9
 *   +0x24 body_fields.maximum_data_size      = 0xFFFFFFFF -> -1
 *   +0x28   field_references[ 0]             = { 0x841831F8 -> &field_properties_update_tick_count_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x38   field_references[ 1]             = { 0x84183D70 -> &field_properties_control_flags_definition, offset 8, baseline_offset 8, initialized 0 }
 *   +0x48   field_references[ 2]             = { 0x84183438 -> &field_properties_real_definition, offset 12, baseline_offset 12, initialized 0 }
 *   +0x58   field_references[ 3]             = { 0x84183438 -> &field_properties_real_definition, offset 16, baseline_offset 16, initialized 0 }
 *   +0x68   field_references[ 4]             = { 0x84183828 -> &field_properties_digital_throttle_definition, offset 32, baseline_offset 32, initialized 0 }
 *   +0x78   field_references[ 5]             = { 0x841835F0 -> &field_properties_fixed_width_1bit_definition, offset 40, baseline_offset 40, initialized 0 }
 *   +0x88   field_references[ 6]             = { 0x84183890 -> &field_properties_fixed_width_weapon_index_definition, offset 44, baseline_offset 44, initialized 0 }
 *   +0x98   field_references[ 7]             = { 0x841838F8 -> &field_properties_fixed_width_grenade_index_definition, offset 46, baseline_offset 46, initialized 0 }
 *   +0xA8   field_references[ 8]             = { 0x84183338 -> &field_properties_integer_medium_definition, offset 48, baseline_offset 48, initialized 0 }
 *   +0xB8   field_references[ 9]             = { 0x00000000, 0, 0, 0 }  <- terminator
 *
 * Layout: DB types_members for _message_definition (40 bytes, 32-bit target) — definition_type,
 * header_size_in_bits, iteration_size_in_bits, iteration_independent_size_in_bits, maximum_total_size,
 * max_iterations, initialized(+3 pad), header_fields, then the inline body_fields set
 * (count, maximum_data_size, field_references[]).
 * Element type and count: body_fields.field_references[] is the struct's trailing flexible array,
 * so the object's real extent fixes its length. This symbol runs to the next .data name at
 * 0x8417DE28, i.e. 200 bytes, and 0x28 + (9 + 1) * 16 == 200 — 16 being the DB member stride of
 * _field_reference_definition. So the array holds body_fields.count live entries plus one all-zero
 * terminator; that identity holds for all 46 message definitions with no exceptions.
 * The four -1 words, `initialized` and body_fields.maximum_data_size are load-time placeholders:
 * message_definition_initialize @0x837A2A50 computes and stores every one of them
 * (stw -> +0x04/+0x08/+0x0C/+0x10/+0x24, stb 1 -> +0x18).
 */
#include "../headers/message_definition.h"
#include "../headers/message_delta_message_ids.h"

extern _message_definition_field_reference_set message_header_client_game_update_header_definition;           /* 0x8417DD38 */
extern _field_properties_definition field_properties_update_tick_count_definition;                                 /* 0x841831F8 */
extern _field_properties_definition field_properties_control_flags_definition;                                     /* 0x84183D70 */
extern _field_properties_definition field_properties_real_definition;                                              /* 0x84183438 */
extern _field_properties_definition field_properties_digital_throttle_definition;                                  /* 0x84183828 */
extern _field_properties_definition field_properties_fixed_width_1bit_definition;                                  /* 0x841835F0 */
extern _field_properties_definition field_properties_fixed_width_weapon_index_definition;                          /* 0x84183890 */
extern _field_properties_definition field_properties_fixed_width_grenade_index_definition;                         /* 0x841838F8 */
extern _field_properties_definition field_properties_integer_medium_definition;                                    /* 0x84183338 */

_message_definition message_client_game_update_definition =
{
    _message_client_game_update,             /* definition_type */
    -1,                                      /* header_size_in_bits */
    -1,                                      /* iteration_size_in_bits */
    -1,                                      /* iteration_independent_size_in_bits */
    -1,                                      /* maximum_total_size */
    1,                                       /* max_iterations */
    0,                                       /* initialized */
    { 0, 0, 0 },                             /* _pad0 */
    &message_header_client_game_update_header_definition, /* header_fields */
    {                                        /* body_fields */
        9,                                   /* count */
        -1,                                  /* maximum_data_size */
        {   /* field_references[count + terminator]: { properties, offset, baseline_offset, initialized } */
            { &field_properties_update_tick_count_definition, 0, 0, 0 },            /* [ 0] */
            { &field_properties_control_flags_definition, 8, 8, 0 },                /* [ 1] */
            { &field_properties_real_definition, 12, 12, 0 },                       /* [ 2] */
            { &field_properties_real_definition, 16, 16, 0 },                       /* [ 3] */
            { &field_properties_digital_throttle_definition, 32, 32, 0 },           /* [ 4] */
            { &field_properties_fixed_width_1bit_definition, 40, 40, 0 },           /* [ 5] */
            { &field_properties_fixed_width_weapon_index_definition, 44, 44, 0 },   /* [ 6] */
            { &field_properties_fixed_width_grenade_index_definition, 46, 46, 0 },  /* [ 7] */
            { &field_properties_integer_medium_definition, 48, 48, 0 },             /* [ 8] */
            { 0, 0, 0, 0 },                                                         /* [ 9] terminator */
        },
    },
};
