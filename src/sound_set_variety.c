/* sound_set_variety @0x83715038 */
#include <stdint.h>
#include "headers/sound_manager_globals.h"

void sound_set_variety(int16_t amount)
{
    sound_manager_globals.sound_variety = amount;
}
