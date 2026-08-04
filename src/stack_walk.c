/* stack_walk @0x83811930 — captures the current call stack (up to 64 frames) into a scratch buffer,
 * skipping levels_to_ignore + 1 frames (the extra +1 skips stack_walk's own frame). */

#include <stdint.h>
#include <string.h>

extern void walk_stack(unsigned int *routine_addresses, unsigned int number_of_levels, unsigned int ignore_levels, unsigned int *levels_dumped);

void stack_walk(int16_t levels_to_ignore)
{
    unsigned int frame_count[4];
    unsigned int frames[64];

    memset(frames, 0, sizeof(frames));
    walk_stack(frames, 0x40u, (__int16)(levels_to_ignore + 1), frame_count);
}
