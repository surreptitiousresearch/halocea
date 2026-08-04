/* ai_scripting_attach_free @0x83771C00 — script command: spawn/attach a "free" AI actor (not bound to any
 * encounter/squad) onto a unit, given an actor-variant tag index directly. The variant tag's data yields the
 * actor definition tag (dword at +0x10); the actor is only created when that definition is non-swarm
 * (actor_def_data[0] & 0x4000000 == 0). No encounter/squad scope: encounter_index and squad_index are -1.
 *
 * The four trailing actor_create_for_unit arguments were lost by the decompiler (it left v3..v6 uninitialized);
 * disasm-verified at 0x83771C70..0x83771C9C: initial_state = actor_default_state_alert, default_state = actor_default_state_none, initial_command_list_index = -1,
 * noncombat_sequence_id = 0. swarm and initially_braindead are both 0. */

#include <stdint.h>
#include "headers/ai_globals.h"
#include "headers/actor_variant_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_default_state.h"
#include "headers/actor_definition_flags.h"

extern int actor_create_for_unit(uint8_t swarm, int unit_index, int actor_variant_definition_index, int encounter_index, int16_t squad_index, uint8_t allow_addition_to_other_squads, int disallow_actor_index, uint8_t initially_braindead, int16_t initial_state, int16_t default_state, int16_t initial_command_list_index, char noncombat_sequence_id);

void ai_scripting_attach_free(int unit_index, int actor_variant_tag_index)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;
    if ( unit_index == -1 || actor_variant_tag_index == -1 )
        return;

    actor_variant_definition *variant = TAG_GET(actor_variant_definition, actor_variant_tag_index);
    int actor_definition_index = variant->actor_reference.index;   /* variant+0x10 */
    if ( actor_definition_index == -1 )
        return;
    if ( (*TAG_GET(int, actor_definition_index) & (1u << _actor_definition_swarm_actor_bit)) != 0 )   /* swarm definitions excluded */
        return;

    actor_create_for_unit(0, unit_index, actor_variant_tag_index, -1, -1, 0, -1, 0, actor_default_state_alert, 0, -1, 0);
}
