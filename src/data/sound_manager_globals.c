/* sound_manager_globals @ 0x846DCFC0 (.data, 188 bytes)
 * DB applied_types: sound_manager_globals_t sound_manager_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 initialized                = 0x00
 *   +0x01 active                     = 0x00
 *   +0x02 paused                     = 0x00
 *   +0x03 idling                     = 0x00
 *   +0x04 game_time_when_no_scripted_dialog_will_be_playing = 0x00000000
 *   +0x08 platform                   = 0x00000000
 *   +0x0C render_time                = 0x00000000
 *   +0x10 ticks_elapsed              = 0x00000000 -> 0f
 *   +0x14 flip_flop                  = 0x00
 *   +0x18 listeners                  = 0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0xA0 sound_environment_tag      = 0x00000000
 *   +0xA4 nondialog_gain             = 0x00000000 -> 0f
 *   +0xA8 music_gain                 = 0x00000000 -> 0f
 *   +0xAC master_gain                = 0x00000000 -> 0f
 *   +0xB0 effects_gain               = 0x00000000 -> 0f
 *   +0xB4 channel_count              = 0x0000
 *   +0xB6 disable_from_commandline   = 0x00
 *   +0xB8 sound_variety              = 0x0000
 * sound manager state block (data-bss, 188 bytes).
 */
#include "../headers/sound_manager_globals_t.h"

sound_manager_globals_t sound_manager_globals;
