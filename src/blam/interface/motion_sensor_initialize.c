#include "headers/blam_data_globals.h"
/* motion_sensor_initialize @ 0x837BCA60 — allocate radar/motion-sensor globals (2776 bytes) */

typedef struct motion_sensor_globals_definition motion_sensor_globals_definition;
extern void *game_state_malloc(const char *name, const char *type, int size);

void motion_sensor_initialize(void)
{
    motion_sensor_globals = game_state_malloc(
        "motion sensor (radar)", "sensor data", 2776);
}
