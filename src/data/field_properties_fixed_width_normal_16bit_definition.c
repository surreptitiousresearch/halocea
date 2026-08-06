/* field_properties_fixed_width_normal_16bit_definition @ 0x84183758 (.data, 104 bytes) — the message-delta field-type descriptor for
 * the "fixed_width_normal_16bit" network field type (_field_type_fixed_width_normal_vector): the encode/decode callback pair the message
 * serializer dispatches through, plus this field's type-specific `parameters` blob.
 * DB applied_types: _field_properties_definition field_properties_fixed_width_normal_16bit_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 type          = 0x00000015 -> _field_type_fixed_width_normal_vector
 *   +0x04 name[75]      = "fixed_width_normal_16bit" (51 trailing bytes zero)
 *   +0x4F _pad4F        = 0x00
 *   +0x50 encode        = 0x8379C8F0 -> default_fixed_width_normal_vector_encoder
 *   +0x54 decode        = 0x8379CA60 -> default_fixed_width_normal_vector_decoder
 *   +0x58 parameters    = 0x84183748 -> &field_properties_fixed_width_normal_16bit_parameters
 *   +0x5C maximum_size  = 0xFFFFFFFF -> -1
 *   +0x60 overhead_bits = 0x00000000 -> 0
 *   +0x64 initialized   = 0x00 (+0x65..0x67 pad = 00 00 00)
 * maximum_size and initialized are load-time placeholders: field_properties_initialize
 * @0x837A11B8 runs this field type's parameter verifier, stores the computed maximum_size and
 * sets initialized = 1.
 * `parameters` is non-NULL, and message_delta_global_field_type_list[21].requires_parameters
 * is 1.
 */

#include "../headers/field_properties_definition.h"
#include "../headers/field_type_fixed_width_normal_vector_parameters.h"

/* Both externs carry the funcptr-SLOT signature from field_properties_definition +0x50/+0x54,
 * NOT the concrete attested one — they exist only to take an address for this dispatch table,
 * and the concrete spelling mismatches the slot (C4113). */
extern int default_fixed_width_normal_vector_encoder(const struct _field_properties_definition *, void *, void *, void *); /* 0x8379C8F0 */
extern int default_fixed_width_normal_vector_decoder(const struct _field_properties_definition *, void *, void *, void *); /* 0x8379CA60 */
extern _field_type_fixed_width_normal_vector_parameters field_properties_fixed_width_normal_16bit_parameters; /* 0x84183748 */

_field_properties_definition field_properties_fixed_width_normal_16bit_definition =
{
    _field_type_fixed_width_normal_vector,        /* type */
    "fixed_width_normal_16bit",                   /* name[75] */
    { 0 },                                        /* _pad4F */
    default_fixed_width_normal_vector_encoder,    /* encode */
    default_fixed_width_normal_vector_decoder,    /* decode */
    &field_properties_fixed_width_normal_16bit_parameters, /* parameters */
    -1,                                           /* maximum_size */
    0,                                            /* overhead_bits */
    0                                             /* initialized */
};
