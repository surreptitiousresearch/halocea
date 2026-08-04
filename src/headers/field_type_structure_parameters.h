#pragma once
/* _field_type_structure_parameters — parameters for the "structure" (nested field-group) message-delta
 * field type: a count followed by a flexible array of field references (each a nested field's own
 * _field_properties_definition plus its byte offset within the destination/baseline structs). DB
 * confirms the base struct is exactly 4 bytes (just `count`); the field array immediately follows. */

#include "field_reference_definition.h"

typedef struct _field_type_structure_parameters
{
    const int field_count;                                     /* 0x00 */
    const _field_reference_definition members_references[];    /* 0x04 — inline flexible array */
} _field_type_structure_parameters;
