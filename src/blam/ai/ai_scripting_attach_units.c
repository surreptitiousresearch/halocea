/* ai_scripting_attach_units @0x8376F520 — script command: spawn/attach an AI actor onto every unit in an object
 * list, using the actor definition of a squad selected by an AI index. This is the multi-unit form of
 * ai_scripting_attach_unit: for each object in the list it performs the identical squad selection + actor creation.
 *
 * The AI index selects an encounter and a squad scope: top two bits >>30 == 2 means the squad index is in byte 1;
 * == 1 means "first squad in the given platoon" (byte 1 = platoon index), searched by the squad platoon field
 * (squad+0x22); otherwise squad 0. The squad's actor type indexes the scenario actor palette to get the
 * actor-variant tag; that tag's data yields the actor definition tag (dword at +0x10).
 *
 * Squad definition fields resolved to the DB squad_definition struct (232 bytes): actor_palette_index,
 * platoon_index, initial_state, default_state. encounter_definition.flags bit 0x10 selects
 * "initially braindead". The four trailing actor_create_for_unit arguments (initial_state, default_state,
 * initial_command_list_index = -1, noncombat_sequence_id = 0) were lost by the decompiler (it left v15..v18
 * uninitialized); they are disasm-verified at 0x8376F678..0x8376F6A4 / 0x8376F548.
 *
 * DEVIATION: the sub-index unpack was `(ai_index >> 8) & 0xFF` — Hex-Rays' BYTE1 expanded with the
 * little-endian value form. Disasm 0x8376F5B4 (explicit squad) and 0x8376F5C8 (platoon search key) are
 * `extrwi rX, r27, 8, 8` (rlwinm SH=16 MB=24 ME=31, words 0x577E863E / 0x5765863E), i.e. (x >> 16) & 0xFF.
 * Now AI_INDEX_SUB_INDEX. */

#include <stdint.h>
#include "headers/ai_index.h"
#include "headers/scenario.h"
#include "headers/encounter_flags.h"
#include "headers/encounter_definition.h"
#include "headers/ai_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/squad_definition.h"
#include "headers/actor_palette_entry.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/blam_data_globals.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern int actor_create_for_unit(uint8_t swarm, int unit_index, int actor_variant_definition_index, int encounter_index, int16_t squad_index, uint8_t allow_addition_to_other_squads, int disallow_actor_index, uint8_t initially_braindead, int16_t initial_state, int16_t default_state, int16_t initial_command_list_index, char noncombat_sequence_id);
extern void encounters_update_dirty_status(void);

void ai_scripting_attach_units(int object_list_index, unsigned int ai_index)
{
    int reference_index;

    for ( int unit_index = object_list_get_first(object_list_index, &reference_index);
          unit_index != -1;
          unit_index = object_list_get_next(object_list_index, &reference_index) )
    {
        if ( !ai_globals->ai_initialized_for_map
          || ai_index == -1
          || (unsigned short)ai_index >= global_scenario->ai_encounters.count )
            continue;

        encounter_definition *encounter =
            &((encounter_definition *)global_scenario->ai_encounters.address)[(unsigned short)ai_index];
        squad_definition *squads = (squad_definition *)encounter->squads.address;
        int squad_index = 0;

        if ( AI_INDEX_SCOPE(ai_index) == _ai_index_squad )
        {
            squad_index = AI_INDEX_SUB_INDEX(ai_index);
        }
        else if ( AI_INDEX_SCOPE(ai_index) == _ai_index_platoon )
        {
            int squad_count = encounter->squads.count;
            int i = 0;
            if ( squad_count > 0 )
            {
                squad_definition *squad = squads;
                while ( squad->platoon_index != AI_INDEX_SUB_INDEX(ai_index) )
                {
                    ++i;
                    ++squad;
                    if ( i >= squad_count )
                        goto have_squad;
                }
                squad_index = i;
            }
        }

    have_squad:
        if ( squad_index < 0 || squad_index >= encounter->squads.count )
            continue;

        squad_definition *squad = &squads[squad_index];
        int16_t actor_type = squad->actor_palette_index;
        if ( actor_type == -1 )
            continue;

        int actor_variant_index =
            ((actor_palette_entry *)global_scenario->ai_actor_palette.address)[actor_type].reference.index;
        if ( actor_variant_index == -1 )
            continue;

        int *variant_tag_data = TAG_GET(int, actor_variant_index);
        int actor_definition_index = variant_tag_data[4];   /* +0x10 */
        if ( actor_definition_index == -1 )
            continue;

        actor_definition *actor_def = TAG_GET(actor_definition, actor_definition_index);
        unsigned char swarm = (actor_def->flags & (1u << _actor_definition_swarm_actor_bit)) != 0;
        unsigned char initially_braindead = (encounter->flags & (1u << _encounter_braindead_bit)) != 0;   /* DB: encounter_definition.flags @0x20 */
        int16_t initial_state = squad->initial_state;
        int16_t default_state = squad->default_state;

        int actor_index = actor_create_for_unit(swarm, unit_index, actor_variant_index, (unsigned short)ai_index,
                                                squad_index, 0, -1, initially_braindead, initial_state, default_state,
                                                -1, 0);
        encounters_update_dirty_status();
    }
}
