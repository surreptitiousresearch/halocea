#pragma once
/* player_effect_datum — per-local-player runtime state for the active player effect (screen flash, camera
 * impulse, camera shake, and continuous controller/camera shake), plus the synthesized impulse direction
 * and jitter axis. Layout from the database (236 bytes, __declspec(align(4))). */

#include <stdint.h>
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
    int16_t                  continuous_effect_timer;  /* 0xDC */
    int16_t                  screen_flash_time_left;   /* 0xDE */
    int16_t                  camera_impulse_time_left; /* 0xE0 */
    int16_t                  camera_shake_time_left;   /* 0xE2 */
    uint8_t          damage_indicator_ticks[4];/* 0xE4 */
    uint8_t          flags;                    /* 0xE8 */
    uint8_t          pad[3];                   /* 0xE9 */
} player_effect_datum;                                 /* 236 bytes */
