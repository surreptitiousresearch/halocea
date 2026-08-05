/* scripted_camera_time @0x83705518 — return the scripted camera's remaining time in ticks (timer seconds
 * * 30). */

#include <stdint.h>
#include "headers/camera_script_globals.h"

int scripted_camera_time(void)
{
    return (uint16_t)(int)(camera_script_globals.timer * 30.0f);
}
