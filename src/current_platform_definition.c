#include "headers/sound_manager_globals.h"
#include "headers/platform_sound_manager_definition.h"

platform_sound_manager_definition * current_platform_definition(void)
{
    return sound_manager_globals.platform;
}
