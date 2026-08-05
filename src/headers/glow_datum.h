#pragma once
/* glow_datum — a live "glow" effect instance (trailing light streaks off object markers). Layout from the
 * database. */

#include <stdint.h>
#include "object_marker.h"

typedef struct glow_particle glow_particle;

typedef struct glow_datum
{
    int16_t          identifier;                                  /* 0x000 */
    uint8_t  initialized;                                  /* 0x002 */
    unsigned char _pad0[1]; /* db-verified padding */
    int16_t          number_of_markers;                            /* 0x004 */
    unsigned char _pad1[2]; /* db-verified padding */
    object_marker    markers[5];                                   /* 0x008 (540 bytes) */
    int              definition_index;                             /* 0x224 */
    int16_t          bitmap_dimension;                             /* 0x228 */
    int16_t          marker_order[5];                              /* 0x22A */
    float            total_time;                                   /* 0x234 */
    float            marker_time_index[5];                         /* 0x238 */
    int16_t          number_of_particles;                          /* 0x24C */
    unsigned char _pad2[2]; /* db-verified padding */
    glow_particle   *head_particle;                                /* 0x250 */
    glow_particle   *tail_particle;                                /* 0x254 */
    int16_t          accumulated_trailing_particle_generation_ticks; /* 0x258 */
} glow_datum;
