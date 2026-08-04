/* actor_action_flush_structure_indices @0x837F1B00 — dispatch to the current action's
 * flush_structure_indices callback (action index at actor+0x6C), tail-calling it with actor_index.
 *
 * DEVIATION: the decompiler casts the callback to `void (*)(void)` and calls it with no arguments; disasm
 * shows this is a genuine tail call (`bctr`) that never touches r3 after entry, so actor_index (r3) is
 * still passed through — matches action_specification.h's real `flush_structure_indices(int actor_index)`
 * signature. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/action_specification.h"
#include "headers/blam_data_globals.h"


void actor_action_flush_structure_indices(uint16_t actor_index)
{
    __int16 action = ((__int16 *)actor_data->data)[914 * actor_index + 54];
    void (__fastcall *flush_structure_indices)(int actor_index) = global_action_functions[action].flush_structure_indices;

    if (flush_structure_indices)
        flush_structure_indices(actor_index);
}
