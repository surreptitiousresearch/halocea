/* input_device_defaults_find @0x838026C8 — locate the first input-device tag ('devc', 0x64657663) whose
 * device type is mouse/keyboard or joystick, and copy its default-bindings block
 * (a player_profile control block) into the destination buffer. Returns the tag index,
 * or -1 if none found.
 * DEVIATION (2026-07-31): two params, not three. The id is a by-value _GUID (r3:r4) that the decompiler
 * split into a _GUID* plus an extra pointer; disasm clobbers r3/r4 without reading them — only the
 * profile buffer (r5) is written. */

#include <stdint.h>
#include "headers/tag_iterator.h"
#include "headers/global_tag_instances.h"
#include "headers/input_device_defaults.h"
#include "headers/player_profile.h"
#include "headers/guid.h"
extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);

extern void *memcpy(void *dst, const void *src, unsigned int count);

int input_device_defaults_find(_GUID id, player_profile *profile)
{
    (void)id;

    tag_iterator iterator;
    tag_iterator_new(&iterator, 0x64657663u);   /* 'devc' */
    int tag_index = tag_iterator_next(&iterator);
    if ( tag_index == -1 )
        return -1;

    input_device_defaults *device;
    while ( 1 )
    {
        device = TAG_GET(input_device_defaults, tag_index);
        if ( device->device_type == _input_device_defaults_device_type_joystick
             || device->device_type == _input_device_defaults_device_type_mouse_keyboard )
            break;
        tag_index = tag_iterator_next(&iterator);
        if ( tag_index == -1 )
            return -1;
    }
    memcpy(profile, device->profile.address, sizeof(player_profile));
    return tag_index;
}
