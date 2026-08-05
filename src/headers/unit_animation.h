#pragma once
/* unit_animation — the animation control block embedded in a unit's object data: which animation graph
 * screens/states are active, the current locomotion/action/overlay states, and the aiming/looking screen
 * setup. Layout from the database. */

#include <stdint.h>
#include "animation_state.h"
#include "real_rectangle2d.h"

typedef struct unit_animation
{
    uint16_t flags;                       /* 0x00 */
    int16_t          aiming_screen_index;         /* 0x02 */
    int16_t          looking_screen_index;        /* 0x04 */
    int16_t          last_ping_animation_index;   /* 0x06 */
    char             seat_index;                  /* 0x08 */
    char             weapon_index;                /* 0x09 */
    char             weapon_type_index;           /* 0x0A */
    char             state;                       /* 0x0B */
    char             action;                      /* 0x0C */
    char             overlay_action;              /* 0x0D */
    char             desired_state;               /* 0x0E */
    char             base_seat_index;             /* 0x0F */
    char             emotion_index;               /* 0x10 */
    char             _pad11;                      /* 0x11 */
    animation_state  action_animation;            /* 0x12 */
    animation_state  overlay_action_animation;    /* 0x16 */
    animation_state  soft_ping_animation;         /* 0x1A */
    uint8_t  aiming_with_euler_screen;    /* 0x1E */
    uint8_t  looking_with_euler_screen;   /* 0x1F */
    real_rectangle2d aiming_screen_bounds;        /* 0x20 */
    real_rectangle2d looking_screen_bounds;       /* 0x30 */
    int              external_animation_graph_index; /* 0x40 */
    animation_state  external_animation;          /* 0x44 */
} unit_animation;                                 /* 0x48 */
