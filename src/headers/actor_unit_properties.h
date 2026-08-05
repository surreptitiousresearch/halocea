#pragma once
#include <stdint.h>
/* actor_unit_properties — actor_variant_definition.unit (32 bytes), DB-verified
 * (types_members type_name='actor_unit_properties'). Per-variant unit overrides. */

typedef struct actor_unit_properties
{
    float           maximum_body_vitality;             /* 0x00 */
    float           maximum_shield_vitality;            /* 0x04 */
    float           shield_sapping_radius;               /* 0x08 */
    int16_t         forced_shader_permutation_index;      /* 0x0C */
    uint16_t pad;                                  /* 0x0E */
    unsigned int    unused[4];                              /* 0x10 */
} actor_unit_properties;                                    /* 0x20 = 32 bytes */
