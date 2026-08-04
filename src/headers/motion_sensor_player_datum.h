#pragma once
/* motion_sensor_player_datum — one player's set of motion-sensor states plus tracked unit indices. */

#include "motion_sensor_datum.h"

typedef struct motion_sensor_player_datum
{
    motion_sensor_datum sensor_data[10];         /* 0x000 */
    int                 current_unit_indices[16]; /* 0x528 */
} motion_sensor_player_datum; /* 1384 bytes */
