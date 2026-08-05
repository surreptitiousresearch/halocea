#pragma once
/* dialogue_variant_definition — one element of _unit_definition.dialogue_variants (24 bytes).
 * Layout from the database (types_members). */

#include <stdint.h>
#include "tag_reference.h"

typedef struct dialogue_variant_definition
{
    int16_t          variant_number;   /* 0x00 */
    uint16_t pad;              /* 0x02 */
    int              unused;           /* 0x04 */
    tag_reference    dialogue_variant; /* 0x08 */
} dialogue_variant_definition;         /* 24 bytes */
