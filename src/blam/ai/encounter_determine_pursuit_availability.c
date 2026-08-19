/* encounter_determine_pursuit_availability @0x83709F38 — decide which target-pursuit behaviours an actor's
 * encounter currently permits. It walks the encounter's actor list (or the encounterless list) and, among actors
 * on the same side of the pursuit-controller split as the caller, tallies how many are pursuing, fleeing, or
 * searching, plus how many are in full combat. From the combat count (and any explicit group_pursuit_restriction)
 * it derives a pursuit cap, then fills the caller's out-flags: whether indefinite target-uncover and target
 * search are allowed (few enough searching/fleeing actors), whether pursuit and pursuit-search are allowed (below
 * the cap), whether this actor is controlling or controlled by a group pursuit (from nearby-actor counts), and
 * whether to wait after pursuit.
 *
 * DEVIATION: all four trailing out-pointers are stack parameters that Hex-Rays scattered across phantom args
 * a13..a34; the DB's 12-arg prototype is authoritative and disasm (0x8370A108-0x8370A1C0 store targets vs the
 * r7/r8/r9/r10 + stack arg order) maps them as allow_pursuit_search / controlling_group_pursuit /
 * controlled_by_group_pursuit / wait_after_pursuit. allow_target_uncover (r7) is never written — the prologue
 * reuses r7 for the encounter lookup. wait_after_pursuit tests whether the two group-pursuit out-pointers are
 * non-null (not their values), as the binary does. The `((x>=0)+(x>=N))&1` flags are the branchless signed
 * "x < N" idiom kept verbatim. The not-initialized path reads an uninitialized stack slot (dead — the loop is
 * gated by ai_initialized_for_map); seeded to -1 here. */

#include <stdint.h>
#include "headers/ai_globals.h"
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/actor_action.h"
#include "headers/group_pursuit_restriction.h"
#include "headers/blam_data_globals.h"


extern int actor_pursuit_find_nearby_actors(int actor_index, int pursuit_coordinator);

void encounter_determine_pursuit_availability(int encounter_index, int actor_index, int16_t group_pursuit_restriction, uint8_t is_pursuit_controller, uint8_t *allow_target_uncover, uint8_t *allow_indefinite_target_uncover, uint8_t *allow_target_search, uint8_t *allow_pursuit, uint8_t *allow_pursuit_search, uint8_t *controlling_group_pursuit, uint8_t *controlled_by_group_pursuit, uint8_t *wait_after_pursuit)
{
    int current_actor = -1;
    if ( ai_globals->ai_initialized_for_map )
    {
        if ( encounter_index == -1 )
            current_actor = ai_globals->first_encounterless_actor_index;
        else
            current_actor = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->first_actor_index;
    }

    int16_t combat_count = 0;
    int16_t fleeing_count = 0;
    int16_t pursuing_count = 0;
    int16_t searching_count = 0;

    while ( ai_globals->ai_initialized_for_map && current_actor != -1 )
    {
        actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, current_actor);
        int is_self = current_actor == actor_index;
        current_actor = actor->meta.next_actor_index;

        if ( !is_self && actor->external_orders.pursuit_is_coordinator == is_pursuit_controller )
        {
            int16_t action = actor->state.action;
            if ( action == actor_action_uncover )
            {
                if ( actor->state.action_data.___u0.uncover.pursuit_location.type )
                    ++pursuing_count;
                else if ( actor->state.combat_status < _actor_combat_status_definite )
                    ++searching_count;
            }
            else if ( action == actor_action_search )
            {
                if ( actor->state.action_data.___u0.search.pursuit_location.type )
                    ++pursuing_count;
                else
                    ++fleeing_count;
            }
        }

        if ( (uint16_t)actor->state.mode == _actor_mode_combat )
            ++combat_count;
    }

    int16_t pursuit_limit;
    if ( group_pursuit_restriction == _group_pursuit_nobody )
    {
        pursuit_limit = 0;
    }
    else if ( group_pursuit_restriction == _group_pursuit_everyone )
    {
        pursuit_limit = 999;   /* effectively-unbounded pursuit cap */
    }
    else
    {
        int limit = combat_count / 3;
        if ( limit < 3 )
            limit = 3;
        pursuit_limit = limit;
    }

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( is_pursuit_controller )
    {
        int nearby = actor_pursuit_find_nearby_actors(actor_index, 1);
        *controlling_group_pursuit = nearby >= 2;
        actor->external_orders.pursuit_is_coordinator = nearby >= 2;
    }
    else
    {
        actor_pursuit_find_nearby_actors(actor_index, 0);
        /* DEVIATION: decompiler garbled the branchless (x+1)!=0 idiom (lwz/addi/addic/subfe @0x8370A11C)
         * into a high-byte pun; the binary computes pursuit_group_prop_index != -1 */
        *controlled_by_group_pursuit = actor->external_orders.pursuit_group_prop_index != -1;
        actor->external_orders.pursuit_is_coordinator = 0;
    }

    *wait_after_pursuit = controlling_group_pursuit != nullptr || controlled_by_group_pursuit != nullptr;

    char pursuit_permitted =
            ((pursuit_limit ^ pursuing_count) >= 0) + (pursuing_count >= (unsigned int)pursuit_limit);
    *allow_indefinite_target_uncover = ((searching_count >= 0) + ((unsigned int)searching_count >= 6)) & 1;
    *allow_target_search = ((fleeing_count >= 0) + ((unsigned int)fleeing_count >= 4)) & 1;
    *allow_pursuit_search = pursuit_permitted & 1;
    *allow_pursuit = pursuit_permitted & 1;
}
