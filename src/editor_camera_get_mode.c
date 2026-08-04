#include <stdint.h>
#include "headers/blam_data_globals.h"
/* editor_camera_get_mode @0x837C3CC8 — getter for the editor camera's current mode (see editor_camera_new.c
 * for the same global). */


int16_t editor_camera_get_mode(void)
{
    return camera_mode;
}
