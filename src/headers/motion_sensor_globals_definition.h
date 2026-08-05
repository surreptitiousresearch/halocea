#pragma once

#include <stdint.h>
#include "motion_sensor_player_datum.h"

typedef struct motion_sensor_globals_definition
{
    motion_sensor_player_datum  motion_sensors[2];  /* 0x000 (2768 bytes) */
    int                         last_update_time;    /* 0xAD0 */
    int16_t                     sensor_active_index; /* 0xAD4 */
    uint8_t             update;              /* 0xAD6 */
    uint8_t             pad_0AD7;            /* 0xAD7 (alignment) */
} motion_sensor_globals_definition;                  /* 2776 bytes */

extern motion_sensor_globals_definition *motion_sensor_globals;
