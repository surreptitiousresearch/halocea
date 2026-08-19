/* sound_get_variety @0x83715048 */
#include "headers/sound_manager_globals.h"

int sound_get_variety(void)
{
    return (unsigned short)sound_manager_globals.sound_variety;
}
