#pragma once
/* unit_animation — the animation control block embedded in a unit's object data: which animation graph
 * screens/states are active, the current locomotion/action/overlay states, and the aiming/looking screen
 * setup. Layout from the database. */

#include "animation_state.h"
#include "real_rectangle2d.h"

typedef struct unit_animation
{
    unsigned __int16 flags;                       /* 0x00 */
    __int16          aiming_screen_index;         /* 0x02 */
    __int16          looking_screen_index;        /* 0x04 */
    __int16          last_ping_animation_index;   /* 0x06 */
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
    unsigned __int8  aiming_with_euler_screen;    /* 0x1E */
    unsigned __int8  looking_with_euler_screen;   /* 0x1F */
    real_rectangle2d aiming_screen_bounds;        /* 0x20 */
    real_rectangle2d looking_screen_bounds;       /* 0x30 */
    int              external_animation_graph_index; /* 0x40 */
    animation_state  external_animation;          /* 0x44 */
} unit_animation;                                 /* 0x48 */
