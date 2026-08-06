/* field_properties_time_definition @ 0x84183DD8 (.data, 104 bytes) — the message-delta field-type descriptor for
 * the "time" network field type (_field_type_time): the encode/decode callback pair the message
 * serializer dispatches through, plus this field's type-specific `parameters` blob.
 * DB applied_types: _field_properties_definition field_properties_time_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 type          = 0x00000012 -> _field_type_time
 *   +0x04 name[75]      = "time" (71 trailing bytes zero)
 *   +0x4F _pad4F        = 0x00
 *   +0x50 encode        = 0x8379C2C0 -> default_time_encoder
 *   +0x54 decode        = 0x8379C2F8 -> default_time_decoder
 *   +0x58 parameters    = 0x00000000 -> NULL
 *   +0x5C maximum_size  = 0xFFFFFFFF -> -1
 *   +0x60 overhead_bits = 0x00000000 -> 0
 *   +0x64 initialized   = 0x00 (+0x65..0x67 pad = 00 00 00)
 * maximum_size and initialized are load-time placeholders: field_properties_initialize
 * @0x837A11B8 runs this field type's parameter verifier, stores the computed maximum_size and
 * sets initialized = 1.
 * `parameters` is NULL, and message_delta_global_field_type_list[18].requires_parameters is 0.
 */

#include "../headers/field_properties_definition.h"

/* Both externs carry the funcptr-SLOT signature from field_properties_definition +0x50/+0x54,
 * NOT the concrete attested one — they exist only to take an address for this dispatch table,
 * and the concrete spelling mismatches the slot (C4113). */
extern int default_time_encoder(const struct _field_properties_definition *, void *, void *, void *); /* 0x8379C2C0 */
extern int default_time_decoder(const struct _field_properties_definition *, void *, void *, void *); /* 0x8379C2F8 */

_field_properties_definition field_properties_time_definition =
{
    _field_type_time,                             /* type */
    "time",                                       /* name[75] */
    { 0 },                                        /* _pad4F */
    default_time_encoder,                         /* encode */
    default_time_decoder,                         /* decode */
    0,                                            /* parameters */
    -1,                                           /* maximum_size */
    0,                                            /* overhead_bits */
    0                                             /* initialized */
};
