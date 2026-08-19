#include <stdint.h>
#include "headers/blam_data_globals.h"
/* editor_camera_get_scripted @0x837C3F40 — getter for the "editor camera is under script control" flag. */


uint8_t editor_camera_get_scripted(void)
{
    return is_scripted;
}
