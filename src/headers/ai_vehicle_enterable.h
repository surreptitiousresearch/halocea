#pragma once
#include <stdint.h>
/* ai_vehicle_enterable — a vehicle registered as "AI-enterable" by script, with the constraints on which
 * actors may enter it (40 bytes). Full DB layout. */

typedef struct ai_vehicle_enterable
{
    int     vehicle_index;       /* 0x00 */
    float   radius;              /* 0x04 — enter-from radius */
    int16_t team_bitmask;        /* 0x08 — allowed teams */
    int16_t actor_type_bitmask;  /* 0x0A — allowed actor types */
    int16_t ai_indices_count;    /* 0x0C */
    uint16_t _pad0E;     /* 0x0E */
    int     ai_indices[6];       /* 0x10 — allowed AI indices */
} ai_vehicle_enterable;          /* 40 bytes */
