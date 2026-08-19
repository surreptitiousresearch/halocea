/* cinematic_suppress_bsp_object_creation @0x83691270 — toggle suppression of BSP-spawned scenery objects
 * during a cinematic (so the cutscene controls object population). */

#include <stdint.h>
#include "headers/cinematic_globals.h"

void cinematic_suppress_bsp_object_creation(uint8_t suppress)
{
    cinematic_globals->cinematic_suppress_bsp_object_creation = suppress;
}
