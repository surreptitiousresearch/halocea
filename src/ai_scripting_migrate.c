/* ai_scripting_migrate @0x837708B0 — migrate all actors of the source encounter into the target encounter
 * (no maneuvering stimulus). */

#include <stdint.h>

extern void ai_scripting_migrate_internal(int source_index, int target_index, uint8_t generate_stimulus, uint8_t advancing_stimulus);

void ai_scripting_migrate(int source_index, int target_index)
{
    ai_scripting_migrate_internal(source_index, target_index, 0, 0);
}
