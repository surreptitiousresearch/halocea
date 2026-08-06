/* default_sound_preferences @ 0x841852C4 (.data, 18 bytes)
 * DB applied_types: sound_preferences default_sound_preferences;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 platform_code              = 0x0000
 *   +0x02 actual_channel_counts      = 0x000A0033000A000A
 *   +0x0A virtual_channel_counts     = 0x0009002E00090009
 */
#include "../headers/sound_preferences.h"

/* .data init @0x841852C4 (sound_preferences, 18 bytes):
 * platform_code=0, actual={10,51,10,10}, virtual={9,46,9,9} */
sound_preferences default_sound_preferences =
{
    0,                  /* platform_code */
    { 10, 51, 10, 10 }, /* actual_channel_counts */
    { 9, 46, 9, 9 },    /* virtual_channel_counts */
};
