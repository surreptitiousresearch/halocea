#pragma once
/* vehicle_state_data — working state for the actor "vehicle" action (enter/ride a vehicle), 76 bytes. One
 * arm of the actor action_state_data union. Full DB layout. */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct vehicle_state_data
{
    int             vehicle_index;              /* 0x00 */
    int16_t         seat_index;                 /* 0x04 */
    uint8_t impromptu;                  /* 0x06 */
    uint8_t ignore_hint;                /* 0x07 */
    uint8_t started_entry;              /* 0x08 */
    uint8_t vehicle_entry_done;         /* 0x09 */
    uint8_t vehicle_entry_failed;       /* 0x0A */
    unsigned char   _pad0B;                     /* 0x0B */
    int16_t         pathfinding_failures;       /* 0x0C */
    int16_t         stuck_detection_counter;    /* 0x0E */
    int             stuck_detection_time;       /* 0x10 */
    real_point3d    stuck_detection_point;      /* 0x14 */
    float           attempt_distance;           /* 0x20 */
    float           continue_distance;          /* 0x24 */
    uint8_t currently_within_range;     /* 0x28 */
    uint8_t currently_correct_facing;   /* 0x29 */
    int16_t         fake_entry_potential_timer; /* 0x2A */
    uint8_t lock_facing;                /* 0x2C */
    unsigned char   _pad2D[3];                  /* 0x2D */
    real_point3d    destination_point;          /* 0x30 */
    real_vector3d   destination_facing;         /* 0x3C */
    int             destination_surface_index;  /* 0x48 */
} vehicle_state_data;                           /* 76 bytes */
