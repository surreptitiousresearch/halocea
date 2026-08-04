#pragma once

/* powered_mass_point_definition — element of physics_definition::powered_mass_points
 * (128 bytes, DB-verified). */

typedef struct powered_mass_point_definition
{
    char         name[32];               /* 0x00 */
    unsigned int flags;                  /* 0x20 */
    float        antigrav_strength;      /* 0x24 */
    float        antigrav_offset;        /* 0x28 */
    float        antigrav_height;        /* 0x2C */
    float        antigrav_damp_fraction; /* 0x30 */
    float        antigrav_normal_k1;     /* 0x34 */
    float        antigrav_normal_k0;     /* 0x38 */
    float        unused[17];             /* 0x3C */
} powered_mass_point_definition;         /* 0x80 = 128 bytes */
