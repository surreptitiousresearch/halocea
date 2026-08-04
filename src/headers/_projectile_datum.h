#pragma once
/* _projectile_datum — the projectile-level runtime fields, embedded at offset 0x22C (556) inside the
 * composite projectile_datum object datum. Layout from the database (132 bytes). */

#include "real_vector3d.h"
#include "projectile_datum_network_data.h"

typedef struct _projectile_datum
{
    unsigned int    flags;                          /* 0x00 — 0x40 = detonation timers randomized (grouped) */
    __int16         action;                         /* 0x04 */
    __int16         hit_material_type;              /* 0x06 */
    int             ignore_object_index;            /* 0x08 */
    int             target_object_index;            /* 0x0C */
    int             tracer_attachment_index_index;  /* 0x10 — slot into object.attachment_indices */
    float           detonation_timer;               /* 0x14 */
    float           detonation_timer_delta;         /* 0x18 */
    float           arming_time;                    /* 0x1C */
    float           arming_time_delta;              /* 0x20 */
    float           odometer;                       /* 0x24 */
    float           deceleration_timer;             /* 0x28 */
    float           deceleration_timer_delta;       /* 0x2C */
    float           deceleration;                   /* 0x30 */
    float           maximum_damage_distance;        /* 0x34 */
    real_vector3d   rotation_axis;                  /* 0x38 */
    float           rotation_sine;                  /* 0x44 */
    float           rotation_cosine;                /* 0x48 */
    unsigned __int8 replicate_detonation;           /* 0x4C */
    unsigned __int8 baseline_valid;                 /* 0x4D */
    unsigned __int8 baseline_index;                 /* 0x4E */
    unsigned __int8 message_index;                  /* 0x4F */
    projectile_datum_network_data baseline;         /* 0x50 */
    unsigned __int8 last_network_data_valid;        /* 0x68 */
    char            _pad69[3];                       /* 0x69 */
    projectile_datum_network_data last_network_data; /* 0x6C */
} _projectile_datum;                                /* 132 bytes */
