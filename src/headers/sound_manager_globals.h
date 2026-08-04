#pragma once

#include <stdint.h>
#include "sound_manager_globals_t.h"
#include "platform_sound_manager_definition.h"
#include "sound_channel_datum.h"

#include "headers/platform_sound_channel_properties.h"
extern sound_manager_globals_t sound_manager_globals;
extern platform_sound_manager_definition *platform_definitions[2];
extern sound_channel_datum sound_channels[256];
extern short sound_channel_type_flags[4];
extern void (*channel_set_properties)(short, platform_sound_channel_properties *, unsigned char, short);
extern void channel_set_properties_hardware(int16_t channel_index, platform_sound_channel_properties *properties, uint8_t gain_only, int16_t class_index);
