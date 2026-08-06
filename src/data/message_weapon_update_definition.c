/* message_weapon_update_definition @ 0x84176430 (.data, 136 bytes) — the message-delta message definition for
 * `_message_weapon_update` (id 3): a weapon's position, velocity, ammunition counters and state.
 * Per-message header set: message_header_game_object_header_definition; 5 body fields.
 *
 * Per-field provenance, decoded big-endian straight from the image bytes:
 *   +0x00 definition_type                    = 0x00000003 -> _message_weapon_update
 *   +0x04 header_size_in_bits                = 0xFFFFFFFF -> -1
 *   +0x08 iteration_size_in_bits             = 0xFFFFFFFF -> -1
 *   +0x0C iteration_independent_size_in_bits = 0xFFFFFFFF -> -1
 *   +0x10 maximum_total_size                 = 0xFFFFFFFF -> -1
 *   +0x14 max_iterations                     = 0x00000001 -> 1
 *   +0x18 initialized                        = 0x00 (+0x19..0x1B pad = 00 00 00)
 *   +0x1C header_fields                      = 0x841845F8 -> &message_header_game_object_header_definition
 *   +0x20 body_fields.count                  = 0x00000005 -> 5
 *   +0x24 body_fields.maximum_data_size      = 0xFFFFFFFF -> -1
 *   +0x28   field_references[ 0]             = { 0x84183C08 -> &field_properties_point3d_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x38   field_references[ 1]             = { 0x841844E8 -> &field_properties_translational_velocity_definition, offset 12, baseline_offset 12, initialized 0 }
 *   +0x48   field_references[ 2]             = { 0x84183338 -> &field_properties_integer_medium_definition, offset 36, baseline_offset 36, initialized 0 }
 *   +0x58   field_references[ 3]             = { 0x84183338 -> &field_properties_integer_medium_definition, offset 38, baseline_offset 38, initialized 0 }
 *   +0x68   field_references[ 4]             = { 0x84183580 -> &field_properties_fixed_width_6bits_definition, offset 40, baseline_offset 40, initialized 0 }
 *   +0x78   field_references[ 5]             = { 0x00000000, 0, 0, 0 }  <- terminator
 *
 * Layout: DB types_members for _message_definition (40 bytes, 32-bit target) — definition_type,
 * header_size_in_bits, iteration_size_in_bits, iteration_independent_size_in_bits, maximum_total_size,
 * max_iterations, initialized(+3 pad), header_fields, then the inline body_fields set
 * (count, maximum_data_size, field_references[]).
 * Element type and count: body_fields.field_references[] is the struct's trailing flexible array,
 * so the object's real extent fixes its length. This symbol runs to the next .data name at
 * 0x841764B8, i.e. 136 bytes, and 0x28 + (5 + 1) * 16 == 136 — 16 being the DB member stride of
 * _field_reference_definition. So the array holds body_fields.count live entries plus one all-zero
 * terminator; that identity holds for all 46 message definitions with no exceptions.
 * The four -1 words, `initialized` and body_fields.maximum_data_size are load-time placeholders:
 * message_definition_initialize @0x837A2A50 computes and stores every one of them
 * (stw -> +0x04/+0x08/+0x0C/+0x10/+0x24, stb 1 -> +0x18).
 */
#include "../headers/message_definition.h"
#include "../headers/message_delta_message_ids.h"

extern _message_definition_field_reference_set message_header_game_object_header_definition;                  /* 0x841845F8 */
extern _field_properties_definition field_properties_point3d_definition;                                           /* 0x84183C08 */
extern _field_properties_definition field_properties_translational_velocity_definition;                            /* 0x841844E8 */
extern _field_properties_definition field_properties_integer_medium_definition;                                    /* 0x84183338 */
extern _field_properties_definition field_properties_fixed_width_6bits_definition;                                 /* 0x84183580 */

_message_definition message_weapon_update_definition =
{
    _message_weapon_update,                  /* definition_type */
    -1,                                      /* header_size_in_bits */
    -1,                                      /* iteration_size_in_bits */
    -1,                                      /* iteration_independent_size_in_bits */
    -1,                                      /* maximum_total_size */
    1,                                       /* max_iterations */
    0,                                       /* initialized */
    { 0, 0, 0 },                             /* _pad0 */
    &message_header_game_object_header_definition, /* header_fields */
    {                                        /* body_fields */
        5,                                   /* count */
        -1,                                  /* maximum_data_size */
        {   /* field_references[count + terminator]: { properties, offset, baseline_offset, initialized } */
            { &field_properties_point3d_definition, 0, 0, 0 },                   /* [ 0] */
            { &field_properties_translational_velocity_definition, 12, 12, 0 },  /* [ 1] */
            { &field_properties_integer_medium_definition, 36, 36, 0 },          /* [ 2] */
            { &field_properties_integer_medium_definition, 38, 38, 0 },          /* [ 3] */
            { &field_properties_fixed_width_6bits_definition, 40, 40, 0 },       /* [ 4] */
            { 0, 0, 0, 0 },                                                      /* [ 5] terminator */
        },
    },
};
