/* ai_scripting_command_list_status_internal @0x83771808 — looks up actor_index's ai_command_list entry
 * (32-byte stride into global_scenario->ai_command_lists) and checks whether *unit_index is a valid index
 * into that entry's command list (count at +0x30, list pointer at +0x34, 32-byte elements); if so returns a
 * status bitmask derived from the resolved element's flags byte, else returns 1 ("not found").
 *
 * command_list_index/simple_control/complex_control are genuine ABI parameters (per the DB's 5-arg
 * prototype) but disasm_range(0x83771808,0x8377185C) confirms this code path never reads r5/r6/r7 — they
 * are dead in this particular early-out body, not decompiler-fabricated, so they are kept in the signature. */

#include <stdint.h>
#include "headers/ai_command_list_definition.h"
#include "headers/ai_command_definition.h"
#include "headers/scenario.h"
#include "headers/obey_individual_simple_control.h"
#include "headers/obey_individual_complex_control.h"

unsigned int ai_scripting_command_list_status_internal(int16_t actor_index, uint8_t *unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control)
{
    uint8_t index = *unit_index;
    ai_command_list_definition_t *command_list = (ai_command_list_definition_t *)global_scenario->ai_command_lists.address + actor_index;

    /* the guarded expression is the resolved element pointer itself (the decompiler emitted it as
     * `(int)address + 32*index`); the 32 is sizeof(ai_command_definition), so index it as one. */
    if ( index < command_list->commands.count
         && &((ai_command_definition *)command_list->commands.address)[index] )
        return (~unit_index[4] >> 4) & 1 | 2;
    return 1;
}
