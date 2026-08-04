/* test_input_device_defaults_find @0x838027F4 — debug console command: look up the default profile tag
 * for a device GUID and print the result.
 *
 * DEVIATION: the disassembly passes input_device_defaults_find a _GUID-by-value and a profile buffer
 * read straight from uninitialized stack (the command supplies no real device id); reproduced
 * faithfully. The "%s" argument is the id_string parameter (the decompiler mislabeled it). */

#include "headers/guid.h"
#include "headers/player_profile.h"

typedef struct real_argb_color real_argb_color;
extern int input_device_defaults_find(_GUID id, player_profile *profile);
extern void terminal_printf(const real_argb_color *color, const char *format, ...);

void test_input_device_defaults_find(const char *id_string)
{
    _GUID device_id;
    player_profile profile;
    int tag_index = input_device_defaults_find(device_id, &profile);
    if ( tag_index == -1 )
        terminal_printf(0, "deviceid %s has no default", id_string);
    else
        terminal_printf(0, "Default profile in tag %d", tag_index);
}
