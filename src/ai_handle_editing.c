/* ai_handle_editing @0x836EA458 — live-editor hook that reconciles the runtime AI state after the game
 * scenario's ai_encounters block has been edited in-place (Sapien-style live editing). Called with the
 * index of the encounter that changed:
 *   - if the index is out of range (or the sentinel high bit is set) the whole AI subsystem is torn down
 *     (kill every actor, dispose communication/script/encounter/prop/actor/path state) and re-initialized
 *     for the (now edited) map;
 *   - otherwise the encounter's squad and platoon counts are compared against the tag definition and the
 *     global squad_array / platoon_array are shifted to grow/shrink this encounter's runs, every following
 *     encounter's squad_base / platoon_base is adjusted by the deltas, and finally each actor in the
 *     encounter is re-validated (reassign or kill on an out-of-range squad, clamp stale platoon /
 *     command-list indices, flush obey-command and firing-position indices).
 *
 * DEVIATIONS (disasm-resolved): (1) the six *_dispose_from_old_map calls and ai_initialize_for_new_map are
 * void(void) — the decompiler's r3 return-value threading (v24=..(); v25=..(v24)) is an artifact (all six
 * are bare `bl` with no argument setup, 0x836EA810-0x836EA834). (2) both squad/platoon memmove calls pass
 * an UNSCALED element count as the byte length, while src/dst ARE scaled by the element size (32 / 16);
 * reproduced verbatim from disasm (0x836EA524-0x836EA538 / 0x836EA5C8-0x836EA5F0) — this is editor-only
 * live-edit code and the quirk is shipped. */

#include <stdint.h>
#include <string.h>
#include "headers/squad_definition.h"
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_definition.h"
#include "headers/actor_datum.h"
#include "headers/actor_iterator.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/ai_globals.h"
#include "headers/squad_datum.h"
#include "headers/platoon_datum.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


extern void actor_iterator_new(actor_iterator *iterator, uint8_t active_only);
extern actor_datum *actor_iterator_next(actor_iterator *iterator);
extern void actor_kill(int actor_index, uint8_t silent, uint8_t delayed);
extern void ai_communication_dispose_from_old_map(void);
extern void ai_script_dispose_from_old_map(void);
extern void encounters_dispose_from_old_map(void);
extern void props_dispose_from_old_map(void);
extern void actors_dispose_from_old_map(void);
extern void paths_dispose_from_old_map(void);
extern void ai_initialize_for_new_map(void);
extern void encounter_actor_iterator_new(encounter_actor_iterator *iterator, int encounter_index);
extern actor_datum *encounter_actor_iterator_next(encounter_actor_iterator *iterator);
extern void action_obey_flush_command_indices(int actor_index);
extern void actor_flush_position_indices(uint16_t actor_index);
/* memset declared by <string.h> */

void ai_handle_editing(int encounter_index)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    scenario *scn = global_scenario;
    int encounter_count = scn->ai_encounters.count;

    if ( (int16_t)encounter_index < 0 || (int16_t)encounter_index >= encounter_count )
    {
        /* Out-of-range / sentinel → tear down and reinitialize the AI subsystem for the edited map. */
        actor_iterator it;
        actor_iterator_new(&it, 0);
        while ( actor_iterator_next(&it) )
            actor_kill(it.index, 0, 0);
        ai_communication_dispose_from_old_map();
        ai_script_dispose_from_old_map();
        encounters_dispose_from_old_map();
        props_dispose_from_old_map();
        actors_dispose_from_old_map();
        paths_dispose_from_old_map();
        ai_globals->ai_initialized_for_map = 0;
        ai_initialize_for_new_map();
        return;
    }

    encounter_datum *enc = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    encounter_definition *enc_def =
        &((encounter_definition *)scn->ai_encounters.address)[(uint16_t)encounter_index];

    int16_t old_squad_count = enc->squad_count;
    int new_squad_count = enc_def->squads.count;
    int16_t squad_delta = (int16_t)(new_squad_count - old_squad_count);
    int16_t platoon_delta = (int16_t)(enc_def->platoons.count - enc->platoon_count);

    if ( squad_delta != 0 )
    {
        encounter_datum *last =
            DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_count - 1);
        /* Shift following encounters' squads; length is an unscaled element count (see DEVIATION 2). */
        memmove(&squad_array[enc->squad_base + new_squad_count],
                &squad_array[enc->squad_base + old_squad_count],
                (int16_t)(last->squad_base + last->squad_count) - enc->squad_base - old_squad_count);
        if ( squad_delta > 0 )
            memset(&squad_array[enc->squad_count + enc->squad_base], 0, 32 * squad_delta);
        enc->squad_count += squad_delta;
    }

    if ( platoon_delta != 0 )
    {
        encounter_datum *last =
            DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, scn->ai_encounters.count - 1);
        /* Shift following encounters' platoons; length is an unscaled element count (see DEVIATION 2). */
        memmove(&platoon_array[enc->platoon_base + enc_def->platoons.count],
                &platoon_array[enc->platoon_base + enc->platoon_count],
                (int16_t)(last->platoon_base + last->platoon_count) - enc->platoon_base - enc->platoon_count);
        if ( platoon_delta > 0 )
            memset(&platoon_array[enc->platoon_base + enc->platoon_count], 0, 16 * platoon_delta);
        enc->platoon_count += platoon_delta;
    }

    if ( squad_delta || platoon_delta )
    {
        for ( int next = (int16_t)(encounter_index + 1); next < scn->ai_encounters.count;
              next = (int16_t)(next + 1) )
        {
            encounter_datum *following = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, next);
            following->platoon_base += platoon_delta;
            following->squad_base += squad_delta;
        }
    }

    /* Re-validate every actor still bound to this encounter. */
    encounter_actor_iterator actor_it;
    encounter_actor_iterator_new(&actor_it, encounter_index);
    for ( actor_datum *actor = encounter_actor_iterator_next(&actor_it); actor;
          actor = encounter_actor_iterator_next(&actor_it) )
    {
        uint8_t kill = 0;
        int squad_index = actor->meta.squad_index;
        if ( squad_index < 0 || squad_index >= enc_def->squads.count )
        {
            if ( enc_def->squads.count )
            {
                /* Reassign to squad 0, inheriting that squad definition's platoon index (word @+34). */
                squad_definition *first_squad = (squad_definition *)enc_def->squads.address;
                actor->meta.squad_index = 0;
                actor->meta.platoon_index = first_squad->platoon_index;
                unsigned int platoon = (uint16_t)first_squad->platoon_index;
                if ( platoon >= 0x8000 || (int16_t)platoon >= enc_def->platoons.count )
                    actor->meta.platoon_index = -1;
            }
            else
            {
                kill = 1;
            }
        }

        if ( kill )
        {
            actor_kill(actor_it.index, 0, 0);
        }
        else
        {
            int platoon_index = actor->meta.platoon_index;
            if ( platoon_index < 0 || platoon_index >= enc_def->platoons.count )
                actor->meta.platoon_index = -1;
            int command_list_index = actor->state.command_list_index;
            if ( command_list_index < 0 || command_list_index >= scn->ai_command_lists.count )
                actor->state.command_list_index = -1;
            if ( actor->state.action == actor_action_obey )
                action_obey_flush_command_indices(actor_it.index);
            actor_flush_position_indices(actor_it.index);
        }
    }
}
