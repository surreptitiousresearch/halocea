#pragma once
/* _field_type_definition — one entry in message_delta_global_field_type_list[28], describing a network
 * field type: size calculation, parameter verification, and teardown callbacks keyed by the field-type id
 * stored at the front of a field's _field_properties_definition. DB layout, 24 bytes. */

#include <stdint.h>
#include "field_properties_definition.h"

typedef struct _field_type_definition
{
    _field_type           type;                  /* 0x00 — _field_type enum id */
    const uint8_t requires_parameters;   /* 0x04 */
    unsigned char         _pad05[3];
    int             (*maximum_size_calculator)(_field_properties_definition *);       /* 0x08 */
    uint8_t (*field_parameters_verifier)(const _field_properties_definition *);/* 0x0C */
    void            (*destructor)(const _field_properties_definition *);              /* 0x10 */
    uint8_t       initialized;           /* 0x14 */
    unsigned char         _pad15[3];
} _field_type_definition;                        /* 24 bytes */
