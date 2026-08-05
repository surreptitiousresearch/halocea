/* actor_action_determine_pursuit_options @0x837F2A60 — resolve, for one actor this tick, which
 * pursuit / target-uncover AI options remain permitted given the group's expressed desires and the
 * actor's current target-prop state. The caller passes four "desire" inputs (target_search / pursuit /
 * pursuit_search as tri-state 0/1/2 plus the two group-pursuit control flags and stand_down) and receives
 * seven output booleans (must_charge + six allow_* flags). A desire of 2 — or 1 while this actor is the
 * controlling group leader — forecloses the matching option; being controlled by another actor's group
 * pursuit forecloses pursuit / pursuit_search / target_uncover. The target prop (actor+624) then refines
 * must_charge and allow_target_search from the prop's tried_to_uncover / tried_to_search flags (falling back
 * to a nearby-firing-position probe when the prop hasn't been searched yet), and late combat-status /
 * awareness-state checks veto a few more.
 *
 * DEVIATION: the 14-param DB prototype is authoritative but two things are misrendered by Hex-Rays:
 *  (1) param 8 `must_charge` is declared by-value u8 yet is really an output POINTER (disasm `stb r,0(r28)`);
 *  (2) Hex-Rays invented phantom params a15..a35 and aliased the six real stack output pointers as
 *      a28/a30/a32/a34/allow_target_uncover_0. Register + stack-offset tracing at 0x837F2A60 maps them:
 *        must_charge            = r28  (param 8, r10 in)
 *        allow_target_uncover   = r27  (0x4C, param 9)
 *        allow_indefinite_target_uncover = arg_54 (0x54, param 10; READ-only here, an input)
 *        allow_target_search    = r26  (0x5C, param 11)
 *        allow_pursuit          = r10/arg_64 (0x64, param 12)
 *        allow_pursuit_search   = r25  (0x6C, param 13)
 *        allow_waiting          = 0x74 (param 14; never touched by this function)
 *      The desire->allow semantic pairing (desire_target_search<->allow_target_search,
 *      desire_pursuit<->allow_pursuit+allow_pursuit_search, desire_pursuit_search<->allow_pursuit_search)
 *      confirms the mapping. `(_cntlzw(x) & 0x20) != 0` is the established "x == 0" boolean idiom
 *      (cntlzw of 0 is 32=0x20), simplified here to a direct comparison. actor fields kept as raw offsets
 *      per sibling convention: +624 target prop index (int), +110 combat_status (__int16), +350 awareness
 *      state (__int16, 0/positive/4). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/real_point3d.h"
#include "headers/actor_combat_status.h"
#include "headers/prop_perception_state.h"
#include "headers/actor_vehicle_driver_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern void actor_perception_find_prop_pathfinding_location(uint16_t actor_index, uint16_t prop_index);
extern uint8_t actor_nearby_firing_positions(int actor_index, real_point3d *test_point, int test_surface_index, int16_t group_selection_mode);

void actor_action_determine_pursuit_options(int actor_index, int16_t desire_target_search, int16_t desire_pursuit, int16_t desire_pursuit_search, uint8_t controlling_group_pursuit, uint8_t controlled_by_group_pursuit, uint8_t stand_down, uint8_t *must_charge, uint8_t *allow_target_uncover, uint8_t *allow_indefinite_target_uncover, uint8_t *allow_target_search, uint8_t *allow_pursuit, uint8_t *allow_pursuit_search, uint8_t *allow_waiting)   /* param 14 — unused by this function; caller default preserved */
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( desire_target_search == 2 || (desire_target_search == 1 && controlling_group_pursuit) )
        *allow_target_search = 0;

    if ( desire_pursuit == 2 || (desire_pursuit == 1 && controlling_group_pursuit) )
    {
        *allow_pursuit = 0;
        *allow_pursuit_search = 0;
    }
    else if ( desire_pursuit_search == 2 || (desire_pursuit_search == 1 && controlling_group_pursuit) )
    {
        *allow_pursuit_search = 0;
    }

    if ( controlled_by_group_pursuit )
    {
        *allow_pursuit = 0;
        *allow_pursuit_search = 0;
        *allow_target_uncover = 0;
    }

    int target_prop_index = actor->target.target_prop_index;
    if ( target_prop_index == -1 )
    {
        *must_charge = 0;
        *allow_target_search = 0;
        return;
    }

    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, target_prop_index);
    *must_charge = prop->tried_to_uncover == 0;
    *allow_target_search = prop->tried_to_search == 0;

    if ( stand_down )
    {
        *must_charge = 0;
    }
    else if ( prop->tried_to_search == 0 )
    {
        if ( prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged )
            actor_perception_find_prop_pathfinding_location(actor_index, target_prop_index);
        *allow_target_search = actor_nearby_firing_positions(actor_index, &prop->pathfinding_point,
                prop->pathfinding_surface_index, 1);
    }

    if ( !*allow_indefinite_target_uncover && actor->state.combat_status < _actor_combat_status_definite )
        *must_charge = 0;

    if ( actor->input.vehicle_driver_type > _actor_vehicle_driver_none )
        *allow_target_uncover = 0;
    if ( (uint16_t)actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying )
    {
        *allow_target_search = 0;
        *allow_pursuit_search = 0;
    }
}
