/* field_properties_map_name_definition @ 0x841816C0 (.data, 104 bytes) — the message-delta field-type descriptor for
 * the "map_name" network field type (_field_type_ascii_string): the encode/decode callback pair the message
 * serializer dispatches through, plus this field's type-specific `parameters` blob.
 * DB applied_types: _field_properties_definition field_properties_map_name_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 type          = 0x00000005 -> _field_type_ascii_string
 *   +0x04 name[75]      = "map_name" (67 trailing bytes zero)
 *   +0x4F _pad4F        = 0x00
 *   +0x50 encode        = 0x8379AB40 -> default_ascii_string_encoder
 *   +0x54 decode        = 0x8379AC10 -> default_ascii_string_decoder
 *   +0x58 parameters    = 0x84181698 -> &field_properties_map_name_parameters
 *   +0x5C maximum_size  = 0xFFFFFFFF -> -1
 *   +0x60 overhead_bits = 0x00000000 -> 0
 *   +0x64 initialized   = 0x00 (+0x65..0x67 pad = 00 00 00)
 * maximum_size and initialized are load-time placeholders: field_properties_initialize
 * @0x837A11B8 runs this field type's parameter verifier, stores the computed maximum_size and
 * sets initialized = 1.
 * `parameters` is non-NULL, and message_delta_global_field_type_list[5].requires_parameters
 * is 1.
 */

#include "../headers/field_properties_definition.h"
#include "../headers/field_type_ascii_string_parameters.h"

/* Both externs carry the funcptr-SLOT signature from field_properties_definition +0x50/+0x54,
 * NOT the concrete attested one — they exist only to take an address for this dispatch table,
 * and the concrete spelling mismatches the slot (C4113). */
extern int default_ascii_string_encoder(const struct _field_properties_definition *, void *, void *, void *); /* 0x8379AB40 */
extern int default_ascii_string_decoder(const struct _field_properties_definition *, void *, void *, void *); /* 0x8379AC10 */
extern _field_type_ascii_string_parameters field_properties_map_name_parameters; /* 0x84181698 */

_field_properties_definition field_properties_map_name_definition =
{
    _field_type_ascii_string,                     /* type */
    "map_name",                                   /* name[75] */
    { 0 },                                        /* _pad4F */
    default_ascii_string_encoder,                 /* encode */
    default_ascii_string_decoder,                 /* decode */
    &field_properties_map_name_parameters,        /* parameters */
    -1,                                           /* maximum_size */
    0,                                            /* overhead_bits */
    0                                             /* initialized */
};
