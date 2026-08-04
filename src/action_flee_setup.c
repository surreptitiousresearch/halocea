/* action_flee_setup @0x83825760 — resets an actor's "flee" behavior state and picks a fleeing strategy. Actors
 * with their offset-352 flag set (swarm/no-precedent-name, same byte as action_guard_setup_find_position)
 * refuse to flee outright. Otherwise: seeds the new target from `panic_prop_index` if given, sets
 * `forced_flee_ticks` to 180 when `force_state_entry` is set, and — for panic types 9-12 — has a 40% chance to
 * just stand and panic in place (`flee_stationary_ticks = 45`, early return) instead of fleeing. If the actor's
 * offset-6 flag is set it also refuses to flee; otherwise it asks `action_flee_find_flee_position` to pick a
 * flee spot and reports failure if none was found.
 *
 * DEVIATION — the DB's own decompile calls `action_flee_find_flee_position(actor_index, (unsigned __int8)state_data, v15)`,
 * which is nonsensical (truncating a pointer to a byte). disasm_range (0x83825858-0x8382586C) shows the real
 * call sets only r3=actor_index and r4=state_data — matching action_flee_find_flee_position's own prologue
 * (`mr r31, r4` binds its 2nd param to the pointer it later dereferences as flee_state_data), so the DB's
 * listed 3-parameter prototype for that callee has its 2nd/3rd argument slots scrambled and its declared 3rd
 * param (whatever it is) is left as caller garbage (leftover `forced_flee_ticks` value, 0 or 180) at this call
 * site — not meaningfully passed. Declared as a 2-arg call here; confirmed directly against
 * action_flee_find_flee_position.c's own prologue disasm. */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/flee_state_data.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>
extern void *memset(void *dst, int value, unsigned int n);

extern uint8_t actor_situation_try_new_target(int actor_index, int prop_index);
extern void action_flee_find_flee_position(int actor_index, flee_state_data *state_data);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);

uint8_t action_flee_setup(int actor_index, int16_t panic_type, int panic_prop_index, uint8_t force_state_entry, uint8_t flee_from_last_visible_location, uint8_t allow_occluded_points, flee_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if (actor->input.vehicle_passenger)
        return 0;

    memset(state_data, 0, sizeof(flee_state_data));
    state_data->panic_type = panic_type;
    state_data->flee_from_last_visible_location = flee_from_last_visible_location;
    state_data->allow_occluded_points = allow_occluded_points;
    state_data->flee_prop_index = panic_prop_index;
    state_data->flee_firing_position_index = -1;
    state_data->forced_flee_ticks = force_state_entry ? 180 : 0;

    if (panic_prop_index != -1)
        actor_situation_try_new_target(actor_index, panic_prop_index);

    if (panic_type >= _actor_panic_grenade_attached_to_us && panic_type <= _actor_panic_burning_to_death)
    {
        if (real_seed_random(get_global_random_seed_address()) < 0.40000001f)
        {
            state_data->flee_stationary_ticks = 45;
            return 1;
        }
    }

    if (actor->meta.swarm)
        return 0;

    action_flee_find_flee_position(actor_index, state_data);

    if ((unsigned __int16)state_data->flee_firing_position_index == 0xFFFF)
    {
        state_data->unable_to_flee = 0;
        return 0;
    }

    return 1;
}
