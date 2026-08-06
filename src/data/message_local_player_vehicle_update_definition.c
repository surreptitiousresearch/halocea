/* message_local_player_vehicle_update_definition @ 0x8417DE90 (.data, 184 bytes) — the message-delta message definition for
 * `_message_local_player_vehicle_update` (id 35): the local player's own vehicle update.
 * The message carries no per-message header (header_fields is the shared empty NO_HEADER set);
 * 8 body fields.
 * Encoder call site cross-checked in headers/message_delta_message_ids.h: 0x83757198 passes id 35.
 *
 * Per-field provenance, decoded big-endian straight from the image bytes:
 *   +0x00 definition_type                    = 0x00000023 -> _message_local_player_vehicle_update
 *   +0x04 header_size_in_bits                = 0xFFFFFFFF -> -1
 *   +0x08 iteration_size_in_bits             = 0xFFFFFFFF -> -1
 *   +0x0C iteration_independent_size_in_bits = 0xFFFFFFFF -> -1
 *   +0x10 maximum_total_size                 = 0xFFFFFFFF -> -1
 *   +0x14 max_iterations                     = 0x00000001 -> 1
 *   +0x18 initialized                        = 0x00 (+0x19..0x1B pad = 00 00 00)
 *   +0x1C header_fields                      = 0x841845B8 -> &message_header_NO_HEADER_definition
 *   +0x20 body_fields.count                  = 0x00000008 -> 8
 *   +0x24 body_fields.maximum_data_size      = 0xFFFFFFFF -> -1
 *   +0x28   field_references[ 0]             = { 0x841830B8 -> &field_properties_local_player_update_sequence_number_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x38   field_references[ 1]             = { 0x84183268 -> &field_properties_update_id_definition, offset 1, baseline_offset 1, initialized 0 }
 *   +0x48   field_references[ 2]             = { 0x84183A30 -> &field_properties_object_index_definition, offset 4, baseline_offset 4, initialized 0 }
 *   +0x58   field_references[ 3]             = { 0x841837C0 -> &field_properties_locality_reference_position_definition, offset 8, baseline_offset 8, initialized 0 }
 *   +0x68   field_references[ 4]             = { 0x841844E8 -> &field_properties_translational_velocity_definition, offset 20, baseline_offset 20, initialized 0 }
 *   +0x78   field_references[ 5]             = { 0x84184160 -> &field_properties_angular_velocity_definition, offset 32, baseline_offset 32, initialized 0 }
 *   +0x88   field_references[ 6]             = { 0x841836E0 -> &field_properties_fixed_width_normal_8bit_definition, offset 44, baseline_offset 44, initialized 0 }
 *   +0x98   field_references[ 7]             = { 0x841836E0 -> &field_properties_fixed_width_normal_8bit_definition, offset 56, baseline_offset 56, initialized 0 }
 *   +0xA8   field_references[ 8]             = { 0x00000000, 0, 0, 0 }  <- terminator
 *
 * Layout: DB types_members for _message_definition (40 bytes, 32-bit target) — definition_type,
 * header_size_in_bits, iteration_size_in_bits, iteration_independent_size_in_bits, maximum_total_size,
 * max_iterations, initialized(+3 pad), header_fields, then the inline body_fields set
 * (count, maximum_data_size, field_references[]).
 * Element type and count: body_fields.field_references[] is the struct's trailing flexible array,
 * so the object's real extent fixes its length. This symbol runs to the next .data name at
 * 0x8417DF48, i.e. 184 bytes, and 0x28 + (8 + 1) * 16 == 184 — 16 being the DB member stride of
 * _field_reference_definition. So the array holds body_fields.count live entries plus one all-zero
 * terminator; that identity holds for all 46 message definitions with no exceptions.
 * The four -1 words, `initialized` and body_fields.maximum_data_size are load-time placeholders:
 * message_definition_initialize @0x837A2A50 computes and stores every one of them
 * (stw -> +0x04/+0x08/+0x0C/+0x10/+0x24, stb 1 -> +0x18).
 */
#include "../headers/message_definition.h"
#include "../headers/message_delta_message_ids.h"

extern _message_definition_field_reference_set message_header_NO_HEADER_definition;                           /* 0x841845B8 */
extern _field_properties_definition field_properties_local_player_update_sequence_number_definition;               /* 0x841830B8 */
extern _field_properties_definition field_properties_update_id_definition;                                         /* 0x84183268 */
extern _field_properties_definition field_properties_object_index_definition;                                      /* 0x84183A30 */
extern _field_properties_definition field_properties_locality_reference_position_definition;                       /* 0x841837C0 */
extern _field_properties_definition field_properties_translational_velocity_definition;                            /* 0x841844E8 */
extern _field_properties_definition field_properties_angular_velocity_definition;                                  /* 0x84184160 */
extern _field_properties_definition field_properties_fixed_width_normal_8bit_definition;                           /* 0x841836E0 */

_message_definition message_local_player_vehicle_update_definition =
{
    _message_local_player_vehicle_update,    /* definition_type */
    -1,                                      /* header_size_in_bits */
    -1,                                      /* iteration_size_in_bits */
    -1,                                      /* iteration_independent_size_in_bits */
    -1,                                      /* maximum_total_size */
    1,                                       /* max_iterations */
    0,                                       /* initialized */
    { 0, 0, 0 },                             /* _pad0 */
    &message_header_NO_HEADER_definition,    /* header_fields */
    {                                        /* body_fields */
        8,                                   /* count */
        -1,                                  /* maximum_data_size */
        {   /* field_references[count + terminator]: { properties, offset, baseline_offset, initialized } */
            { &field_properties_local_player_update_sequence_number_definition, 0, 0, 0 },  /* [ 0] */
            { &field_properties_update_id_definition, 1, 1, 0 },                            /* [ 1] */
            { &field_properties_object_index_definition, 4, 4, 0 },                         /* [ 2] */
            { &field_properties_locality_reference_position_definition, 8, 8, 0 },          /* [ 3] */
            { &field_properties_translational_velocity_definition, 20, 20, 0 },             /* [ 4] */
            { &field_properties_angular_velocity_definition, 32, 32, 0 },                   /* [ 5] */
            { &field_properties_fixed_width_normal_8bit_definition, 44, 44, 0 },            /* [ 6] */
            { &field_properties_fixed_width_normal_8bit_definition, 56, 56, 0 },            /* [ 7] */
            { 0, 0, 0, 0 },                                                                 /* [ 8] terminator */
        },
    },
};
