#pragma once
/* director / director_globals — drives each local player's camera each frame (director.c). The director
 * owns the active camera proc and its opaque per-camera state blob, and accumulates the resolved
 * observer_command for the player. */

#include "observer_command.h"
#include "camera_control.h"
#include "director_variable_instance.h"

typedef struct director
{
    __int16 camera_mode_index;              /* 0x00 */
    unsigned char _pad02[2];                /* 0x02 */
    float   camera_change_pause;            /* 0x04 — countdown blending out of a camera change */
    void (__fastcall *camera_proc)(void *camera_data, const camera_control *, observer_command *); /* 0x08 */
    unsigned __int8 camera_data[64];        /* 0x0C — opaque per-camera state */
    int     bored_time;                     /* 0x4C */
    unsigned __int8 bored;                  /* 0x50 */
    unsigned __int8 inhibited_facing;       /* 0x51 */
    unsigned __int8 inhibited_input;        /* 0x52 */
    unsigned char _pad53[1];                /* 0x53 */
    __int16 seat_state;                     /* 0x54 */
    __int16 perspective;                    /* 0x56 */
    observer_command command;               /* 0x58 */
    unsigned __int8 debug_controls;         /* 0xC0 */
    unsigned char _padC1[3];                /* 0xC1 */
    float   debug_input_scale;              /* 0xC4 */
    director_variable_instance debug_variables[4]; /* 0xC8 */
} director;                                 /* 248 bytes */

/* DB spells the globals blob as anonymous struct _15F3ED9EB994D8D3C43F28ECD84A8394
 * (types_members: dtime@0, game_mode@4, initialize_camera@6, director local_players[2]@8).
 * $-name kept verbatim ($ in identifiers is an MSVC extension — argb_color.h convention). */
typedef struct _15F3ED9EB994D8D3C43F28ECD84A8394
{
    float   dtime;                          /* 0x00 */
    __int16 game_mode;                      /* 0x04 */
    unsigned __int8 initialize_camera;      /* 0x06 */
    unsigned char _pad07[1];                /* 0x07 */
    director local_players[2];              /* 0x08 */
} _15F3ED9EB994D8D3C43F28ECD84A8394;                                          /* 504 bytes */
typedef struct _15F3ED9EB994D8D3C43F28ECD84A8394 director_globals_t;

extern director_globals_t director_globals;
