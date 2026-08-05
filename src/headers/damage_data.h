#pragma once
/* damage_data — a damage event description passed to object_cause_damage / effect systems. Layout from
 * the database. */

#include <stdint.h>
#include "location.h"
#include "real_point3d.h"
#include "real_vector3d.h"

struct projectile_material_response_definition;

typedef struct damage_data
{
    int            definition_index;       /* 0x00 */
    unsigned int   flags;                  /* 0x04 */
    int            owner_player_index;      /* 0x08 */
    int            owner_object_index;      /* 0x0C */
    int16_t        owner_team_index;        /* 0x10 */
    char           _pad12[2];               /* 0x12 */
    location       location;                /* 0x14 */
    real_point3d   origin;                  /* 0x1C */
    real_point3d   epicenter;               /* 0x28 */
    real_vector3d  direction;               /* 0x34 */
    float          scale;                   /* 0x40 */
    float          multiplier;              /* 0x44 */
    float          material_effect_scale;   /* 0x48 */
    int16_t        material_type;           /* 0x4C */
    char           _pad4E[2];               /* 0x4E */
    const struct projectile_material_response_definition *material_response; /* 0x50 */
} damage_data;
