#pragma once
/* _field_reference_definition — one field within a message-delta message's header or body: the field-type
 * properties (encode/decode), the byte offset of the field's value within the message struct, and the byte
 * offset of its baseline value (incremental mode). */

#include <stdint.h>
#include "field_properties_definition.h"

typedef struct _field_reference_definition
{
    _field_properties_definition *const properties;        /* 0x0 */
    const int                           offset;            /* 0x4 */
    const int                           baseline_offset;   /* 0x8 */
    uint8_t                     initialized;       /* 0xC */
} _field_reference_definition;
