#pragma once
/* player_effect_datum — per-local-player runtime state for the active player effect (screen flash, camera
 * impulse, camera shake, and continuous controller/camera shake), plus the synthesized impulse direction
 * and jitter axis. Layout from the database (236 bytes, __declspec(align(4))). */

#include "real_vector3d.h"
#include "screen_flash_definition.h"
#include "camera_impulse_definition.h"
#include "camera_shake_definition.h"
#include "continuous_player_effect_datum.h"

typedef struct player_effect_datum
{
    real_vector3d            direction;                /* 0x00 */
    real_vector3d            jitter;                   /* 0x0C */
    screen_flash_definition  screen_flash;             /* 0x18 */
    camera_impulse_definition camera_impulse;          /* 0x50 */
    camera_shake_definition  camera_shake;             /* 0x84 */
    continuous_player_effect_datum continuous_effect;  /* 0xCC */
    __int16                  continuous_effect_timer;  /* 0xDC */
    __int16                  screen_flash_time_left;   /* 0xDE */
    __int16                  camera_impulse_time_left; /* 0xE0 */
    __int16                  camera_shake_time_left;   /* 0xE2 */
    unsigned __int8          damage_indicator_ticks[4];/* 0xE4 */
    unsigned __int8          flags;                    /* 0xE8 */
    unsigned __int8          pad[3];                   /* 0xE9 */
} player_effect_datum;                                 /* 236 bytes */
