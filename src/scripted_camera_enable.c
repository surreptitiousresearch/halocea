/* scripted_camera_enable @0x83704DC0 — enable/disable the scripted camera and force the next update to be
 * treated as the first (so it snaps rather than interpolates). */

#include "headers/camera_script_globals.h"
#include <stdint.h>

/* attested: uint8_t enabled — stb r3 into camera_script_globals.enabled @0x83704DCC */
void scripted_camera_enable(uint8_t enabled)
{
    camera_script_globals.enabled = enabled;
    camera_script_globals.first_update = 1;
}
