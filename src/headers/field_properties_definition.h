#pragma once
/* _field_properties_definition — describes one network-serialized field "type": its name, encode/decode
 * callbacks, the type-specific `parameters` blob, and serialized-size accounting. For index-translated
 * fields the parameters blob begins with an index_resolution_table at +12.
 *
 * DEVIATION: `type` was previously modeled as a pointer, but DB types_members confirms it's a plain 4-byte
 * `_field_type` enum value (used directly as an index into message_delta_global_field_type_list[28]). */

#include "_field_type.h"   /* enum modeled in DB (types_enum_values _field_type) */

typedef struct _field_properties_definition
{
    _field_type               type;          /* 0x00 */
    char                      name[75];       /* 0x04 */
    unsigned char             _pad4F[1];      /* 0x4F */
    int (*encode)(const struct _field_properties_definition *, void *, void *, void *); /* 0x50 */
    int (*decode)(const struct _field_properties_definition *, void *, void *, void *); /* 0x54 */
    void *const               parameters;     /* 0x58 */
    int                       maximum_size;    /* 0x5C */
    int                       overhead_bits;   /* 0x60 */
    unsigned __int8           initialized;     /* 0x64 */
} _field_properties_definition;
