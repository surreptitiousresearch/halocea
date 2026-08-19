/* sound_disable_from_commandline @ 0x83715368 */

#include <stdint.h>
#include "headers/sound_manager_globals.h"

/* attested: param stored via stb (8-bit) @ 0x83715370 => uint8_t */
void sound_disable_from_commandline(uint8_t disable)
{
    sound_manager_globals.disable_from_commandline = disable;
}
