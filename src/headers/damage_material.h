#pragma once
#include <stdint.h>
/* damage_material — a per-material damage-resistance record (72-byte stride) in the damage_resistance
 * materials block. Layout from the database. */

typedef struct damage_material
{
    char             name[32];                /* 0x00 */
    unsigned int     flags;                   /* 0x20 — 0x20 = cannot-take-damage-this-way */
    int16_t          type;                     /* 0x24 */
    uint16_t pad;                      /* 0x26 */
    float            shield_leak_fraction;     /* 0x28 */
    float            shield_damage_multiplier; /* 0x2C */
    float            shield_unused[3];         /* 0x30 */
    float            body_damage_multiplier;   /* 0x3C */
    int              body_unused[2];           /* 0x40 */
} damage_material;                             /* 0x48 (72 bytes) */
