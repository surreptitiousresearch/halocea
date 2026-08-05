#pragma once
/* animation_thread — a runtime thread that drives a unit through a recorded animation by streaming events
 * into a unit_control_data. One datum (stride 100) in the animation_threads pool. Layout from the database;
 * the embedded controller and animation_state are their own typed structs. */

#include <stdint.h>
#include "unit_control_data.h"
#include "animation_playback_controller.h"

typedef struct animation_thread
{
    int16_t                       identifier;      /* 0x00 */
    char                          _pad02[2];       /* 0x02 */
    int                           unit_index;      /* 0x04 */
    uint16_t              ticks_left;      /* 0x08 */
    uint16_t              flags;           /* 0x0A */
    int                           relative_ticks;  /* 0x0C */
    const char                   *event_stream;    /* 0x10 */
    unit_control_data             controller;      /* 0x14 */
    animation_playback_controller animation_state; /* 0x54 */
    int16_t                       version;         /* 0x60 */
    char                          _pad62[2];       /* 0x62 */
} animation_thread;                                /* 100 bytes */
