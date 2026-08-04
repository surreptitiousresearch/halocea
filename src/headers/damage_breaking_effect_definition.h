#pragma once
/* damage_breaking_effect_definition — breakable-surface impulse sub-block of the 'jpt!' damage
 * effect tag (DB types_members damage_breaking_effect_definition, 48 bytes). */

typedef struct damage_breaking_effect_definition
{
    float forward_velocity;  /* 0x00 */
    float forward_radius;    /* 0x04 */
    float forward_exponent;  /* 0x08 */
    int   forward_unused[3]; /* 0x0C */
    float outward_velocity;  /* 0x18 */
    float outward_radius;    /* 0x1C */
    float outward_exponent;  /* 0x20 */
    int   outward_unused[3]; /* 0x24 */
} damage_breaking_effect_definition; /* 48 bytes */
