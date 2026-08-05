#pragma once
#include <stdint.h>
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* vehicle_scripting_candidate — one actor considered for a scripted "go to vehicle" order (12 bytes).
 * The candidates are gathered, sorted (actors not already heading to a vehicle first, then by ascending
 * distance to the vehicle), and dispatched in order. This is a dedicated struct for the 12-byte stride the
 * decompiler treats as a raw byte buffer. */

typedef struct vehicle_scripting_candidate
{
    int             actor_index;                /* 0x00 */
    float           distance_squared;           /* 0x04 — squared distance to the vehicle */
    uint8_t already_going_to_vehicle;   /* 0x08 — actor already running the vehicle action */
    unsigned char   _pad09[3];                  /* 0x09 */
} vehicle_scripting_candidate;                  /* 12 bytes */
