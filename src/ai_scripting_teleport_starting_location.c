/* ai_scripting_teleport_starting_location @0x83771370 — teleport all actors under an AI index back to
 * their starting locations unconditionally. */

#include <stdint.h>

extern void ai_scripting_teleport_starting_location_private(int ai_index, uint8_t only_if_unsupported);

void ai_scripting_teleport_starting_location(int ai_index)
{
    ai_scripting_teleport_starting_location_private(ai_index, 0);
}
