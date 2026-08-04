#pragma once
/* _field_type_array_parameters — parameters for the "array" message-delta field type: a fixed-size
 * array of identically-typed elements, each encoded/decoded via the same shared field_properties.
 * DB-confirmed 12 bytes. */

#include "field_properties_definition.h"

typedef struct _field_type_array_parameters
{
    int                            count;              /* 0x00 */
    int                            source_element_size; /* 0x04 */
    _field_properties_definition *member_properties;  /* 0x08 */
} _field_type_array_parameters; /* 12 bytes */
