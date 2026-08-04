#pragma once
/* _message_definition_field_reference_set — a set of field references making up either the header or body of
 * a message-delta message definition. `field_references` is a trailing flexible array. */

#include "field_reference_definition.h"

typedef struct _message_definition_field_reference_set
{
    const int count;              /* 0x0 */
    int       maximum_data_size;  /* 0x4 */
    _field_reference_definition field_references[];   /* 0x8 — inline flexible array */
} _message_definition_field_reference_set;
