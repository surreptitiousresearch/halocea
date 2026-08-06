/* message_remote_player_action_update_definition @ 0x8417DF90 (.data, 168 bytes) — the message-delta message definition for
 * `_message_remote_player_action_update` (id 36): a remote player's control/action update.
 * Per-message header set: message_header_remote_player_action_update_header_definition; 7 body fields.
 * Encoder call site cross-checked in headers/message_delta_message_ids.h: 0x83757AD8 passes id 36.
 *
 * Per-field provenance, decoded big-endian straight from the image bytes:
 *   +0x00 definition_type                    = 0x00000024 -> _message_remote_player_action_update
 *   +0x04 header_size_in_bits                = 0xFFFFFFFF -> -1
 *   +0x08 iteration_size_in_bits             = 0xFFFFFFFF -> -1
 *   +0x0C iteration_independent_size_in_bits = 0xFFFFFFFF -> -1
 *   +0x10 maximum_total_size                 = 0xFFFFFFFF -> -1
 *   +0x14 max_iterations                     = 0x00000001 -> 1
 *   +0x18 initialized                        = 0x00 (+0x19..0x1B pad = 00 00 00)
 *   +0x1C header_fields                      = 0x8417DF48 -> &message_header_remote_player_action_update_header_definition
 *   +0x20 body_fields.count                  = 0x00000007 -> 7
 *   +0x24 body_fields.maximum_data_size      = 0xFFFFFFFF -> -1
 *   +0x28   field_references[ 0]             = { 0x841831F8 -> &field_properties_update_tick_count_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x38   field_references[ 1]             = { 0x84183D70 -> &field_properties_control_flags_definition, offset 4, baseline_offset 4, initialized 0 }
 *   +0x48   field_references[ 2]             = { 0x841836E0 -> &field_properties_fixed_width_normal_8bit_definition, offset 36, baseline_offset 36, initialized 0 }
 *   +0x58   field_references[ 3]             = { 0x84183828 -> &field_properties_digital_throttle_definition, offset 16, baseline_offset 16, initialized 0 }
 *   +0x68   field_references[ 4]             = { 0x841835F0 -> &field_properties_fixed_width_1bit_definition, offset 24, baseline_offset 24, initialized 0 }
 *   +0x78   field_references[ 5]             = { 0x84183890 -> &field_properties_fixed_width_weapon_index_definition, offset 28, baseline_offset 28, initialized 0 }
 *   +0x88   field_references[ 6]             = { 0x841838F8 -> &field_properties_fixed_width_grenade_index_definition, offset 30, baseline_offset 30, initialized 0 }
 *   +0x98   field_references[ 7]             = { 0x00000000, 0, 0, 0 }  <- terminator
 *
 * Layout: DB types_members for _message_definition (40 bytes, 32-bit target) — definition_type,
 * header_size_in_bits, iteration_size_in_bits, iteration_independent_size_in_bits, maximum_total_size,
 * max_iterations, initialized(+3 pad), header_fields, then the inline body_fields set
 * (count, maximum_data_size, field_references[]).
 * Element type and count: body_fields.field_references[] is the struct's trailing flexible array,
 * so the object's real extent fixes its length. This symbol runs to the next .data name at
 * 0x8417E038, i.e. 168 bytes, and 0x28 + (7 + 1) * 16 == 168 — 16 being the DB member stride of
 * _field_reference_definition. So the array holds body_fields.count live entries plus one all-zero
 * terminator; that identity holds for all 46 message definitions with no exceptions.
 * The four -1 words, `initialized` and body_fields.maximum_data_size are load-time placeholders:
 * message_definition_initialize @0x837A2A50 computes and stores every one of them
 * (stw -> +0x04/+0x08/+0x0C/+0x10/+0x24, stb 1 -> +0x18).
 */
#include "../headers/message_definition.h"
#include "../headers/message_delta_message_ids.h"

extern _message_definition_field_reference_set message_header_remote_player_action_update_header_definition;  /* 0x8417DF48 */
extern _field_properties_definition field_properties_update_tick_count_definition;                                 /* 0x841831F8 */
extern _field_properties_definition field_properties_control_flags_definition;                                     /* 0x84183D70 */
extern _field_properties_definition field_properties_fixed_width_normal_8bit_definition;                           /* 0x841836E0 */
extern _field_properties_definition field_properties_digital_throttle_definition;                                  /* 0x84183828 */
extern _field_properties_definition field_properties_fixed_width_1bit_definition;                                  /* 0x841835F0 */
extern _field_properties_definition field_properties_fixed_width_weapon_index_definition;                          /* 0x84183890 */
extern _field_properties_definition field_properties_fixed_width_grenade_index_definition;                         /* 0x841838F8 */

_message_definition message_remote_player_action_update_definition =
{
    _message_remote_player_action_update,    /* definition_type */
    -1,                                      /* header_size_in_bits */
    -1,                                      /* iteration_size_in_bits */
    -1,                                      /* iteration_independent_size_in_bits */
    -1,                                      /* maximum_total_size */
    1,                                       /* max_iterations */
    0,                                       /* initialized */
    { 0, 0, 0 },                             /* _pad0 */
    &message_header_remote_player_action_update_header_definition, /* header_fields */
    {                                        /* body_fields */
        7,                                   /* count */
        -1,                                  /* maximum_data_size */
        {   /* field_references[count + terminator]: { properties, offset, baseline_offset, initialized } */
            { &field_properties_update_tick_count_definition, 0, 0, 0 },            /* [ 0] */
            { &field_properties_control_flags_definition, 4, 4, 0 },                /* [ 1] */
            { &field_properties_fixed_width_normal_8bit_definition, 36, 36, 0 },    /* [ 2] */
            { &field_properties_digital_throttle_definition, 16, 16, 0 },           /* [ 3] */
            { &field_properties_fixed_width_1bit_definition, 24, 24, 0 },           /* [ 4] */
            { &field_properties_fixed_width_weapon_index_definition, 28, 28, 0 },   /* [ 5] */
            { &field_properties_fixed_width_grenade_index_definition, 30, 30, 0 },  /* [ 6] */
            { 0, 0, 0, 0 },                                                         /* [ 7] terminator */
        },
    },
};
