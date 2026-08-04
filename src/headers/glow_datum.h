#pragma once
/* glow_datum — a live "glow" effect instance (trailing light streaks off object markers). Layout from the
 * database. */

#include "object_marker.h"

typedef struct glow_particle glow_particle;

typedef struct glow_datum
{
    __int16          identifier;                                  /* 0x000 */
    unsigned __int8  initialized;                                  /* 0x002 */
    unsigned char _pad0[1]; /* db-verified padding */
    __int16          number_of_markers;                            /* 0x004 */
    unsigned char _pad1[2]; /* db-verified padding */
    object_marker    markers[5];                                   /* 0x008 (540 bytes) */
    int              definition_index;                             /* 0x224 */
    __int16          bitmap_dimension;                             /* 0x228 */
    __int16          marker_order[5];                              /* 0x22A */
    float            total_time;                                   /* 0x234 */
    float            marker_time_index[5];                         /* 0x238 */
    __int16          number_of_particles;                          /* 0x24C */
    unsigned char _pad2[2]; /* db-verified padding */
    glow_particle   *head_particle;                                /* 0x250 */
    glow_particle   *tail_particle;                                /* 0x254 */
    __int16          accumulated_trailing_particle_generation_ticks; /* 0x258 */
} glow_datum;
