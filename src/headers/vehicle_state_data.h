#pragma once
/* vehicle_state_data — working state for the actor "vehicle" action (enter/ride a vehicle), 76 bytes. One
 * arm of the actor action_state_data union. Full DB layout. */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct vehicle_state_data
{
    int             vehicle_index;              /* 0x00 */
    __int16         seat_index;                 /* 0x04 */
    unsigned __int8 impromptu;                  /* 0x06 */
    unsigned __int8 ignore_hint;                /* 0x07 */
    unsigned __int8 started_entry;              /* 0x08 */
    unsigned __int8 vehicle_entry_done;         /* 0x09 */
    unsigned __int8 vehicle_entry_failed;       /* 0x0A */
    unsigned char   _pad0B;                     /* 0x0B */
    __int16         pathfinding_failures;       /* 0x0C */
    __int16         stuck_detection_counter;    /* 0x0E */
    int             stuck_detection_time;       /* 0x10 */
    real_point3d    stuck_detection_point;      /* 0x14 */
    float           attempt_distance;           /* 0x20 */
    float           continue_distance;          /* 0x24 */
    unsigned __int8 currently_within_range;     /* 0x28 */
    unsigned __int8 currently_correct_facing;   /* 0x29 */
    __int16         fake_entry_potential_timer; /* 0x2A */
    unsigned __int8 lock_facing;                /* 0x2C */
    unsigned char   _pad2D[3];                  /* 0x2D */
    real_point3d    destination_point;          /* 0x30 */
    real_vector3d   destination_facing;         /* 0x3C */
    int             destination_surface_index;  /* 0x48 */
} vehicle_state_data;                           /* 76 bytes */
