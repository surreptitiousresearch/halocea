#pragma once
/* motion_sensor_globals — runtime motion-sensor (radar) state.
 * Types split into per-type headers (tiny_point2d / motion_sensor_blip / motion_sensor_datum /
 * motion_sensor_player_datum / motion_sensor_globals_definition); this header aggregates them and
 * declares the global instance pointer. */

#include "motion_sensor_globals_definition.h"

extern motion_sensor_globals_definition *motion_sensor_globals;
