/* ai_scripting_link_activation @0x837727A8 — link one encounter's activation to another, so activating the
 * target also activates the linked encounter. No-op if either index is invalid. */

#include <stdint.h>

extern int encounter_link_activation(uint16_t encounter_index, int16_t link_encounter_index);

void ai_scripting_link_activation(int target_ai_index, int link_ai_index)
{
    if ( target_ai_index != -1 && link_ai_index != -1 )
        encounter_link_activation((uint16_t)target_ai_index, link_ai_index);
}
