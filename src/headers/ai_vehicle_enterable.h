#pragma once
/* ai_vehicle_enterable — a vehicle registered as "AI-enterable" by script, with the constraints on which
 * actors may enter it (40 bytes). Full DB layout. */

typedef struct ai_vehicle_enterable
{
    int     vehicle_index;       /* 0x00 */
    float   radius;              /* 0x04 — enter-from radius */
    __int16 team_bitmask;        /* 0x08 — allowed teams */
    __int16 actor_type_bitmask;  /* 0x0A — allowed actor types */
    __int16 ai_indices_count;    /* 0x0C */
    unsigned __int16 _pad0E;     /* 0x0E */
    int     ai_indices[6];       /* 0x10 — allowed AI indices */
} ai_vehicle_enterable;          /* 40 bytes */
