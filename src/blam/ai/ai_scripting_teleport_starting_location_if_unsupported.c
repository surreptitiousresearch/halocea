/* ai_scripting_teleport_starting_location_if_unsupported @0x83771368 — teleport actors under an AI index
 * back to their starting locations only if they are unsupported (on foot with no ground surface beneath). */

#include <stdint.h>

extern void ai_scripting_teleport_starting_location_private(int ai_index, uint8_t only_if_unsupported);

void ai_scripting_teleport_starting_location_if_unsupported(int ai_index)
{
    ai_scripting_teleport_starting_location_private(ai_index, 1u);
}
