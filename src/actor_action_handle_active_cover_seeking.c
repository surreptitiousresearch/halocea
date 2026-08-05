/* actor_action_handle_active_cover_seeking @0x837F53D0 — reconsider taking cover while the actor is
 * actively in danger (+76 flag), once its distance-to-threat (+444) exceeds its definition's cover-seek
 * distance threshold (tag def word offset 732). Only re-evaluates while mid combat-status (>= 2) in a
 * pursuit or active action class (_action_class_pursuit / _action_class_active) and not already suppressed (+888), and at most once every
 * 30+ ticks. Attempts to seek cover (preferring the last-visible-location cover point) and, failing that,
 * falls back to panicking if allow_panicking is set. Returns whether either action succeeded.
 *
 * Note: the danger/threat-distance/suppression/last-attempt fields (decompiler offsets +76/+444/+888/+880)
 * are read through named actor_datum members in the body; +108/+110 are state.action/combat_status and
 * +624 the target prop index. This function looks up the actor's own definition tag
 * (meta.definition_index), not its variant tag. The tag-def cover-seek threshold at word offset 732 is
 * read through definition_tag->defensive.shield_fraction_hide. */

#include <stdint.h>
#include "headers/actor_definition.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/action_specification.h"
#include "headers/action_class.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_panic_type.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern uint8_t actor_action_allow_cover_seeking(int actor_index, uint8_t unopposable);
extern uint8_t actor_action_try_to_seek_cover(int actor_index, uint8_t cover_from_last_visible_location, uint8_t allow_occluded_points);
extern uint8_t actor_action_try_to_panic(int actor_index, int16_t panic_type, int panic_prop_index,
    uint8_t force_flee_transition);

int actor_action_handle_active_cover_seeking(int actor_index, uint8_t allow_panicking, uint8_t force_panicking)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( !actor->meta.timeslice )
        return 0;

    const actor_definition *definition_tag = TAG_GET(const actor_definition, actor->meta.definition_index);

    /* cover-seek threshold: definition_tag->defensive.shield_fraction_hide (actor_definition tag+732). */
    if ( actor->input.recent_shield_damage > definition_tag->defensive.shield_fraction_hide )
        return 0;

    int16_t action_class = global_action_functions[actor->state.action].action_class;

    if ( actor->emotions.berserk || (action_class != _action_class_active && action_class != _action_class_pursuit) || actor->state.combat_status < _actor_combat_status_investigate )
        return 0;

    int current_time = game_time_get();

    int *last_attempt_time_ref = &actor->emotions.last_active_cover_seeking_time;
    if ( *last_attempt_time_ref != -1 && current_time < *last_attempt_time_ref + 30 )
        return 0;

    *last_attempt_time_ref = current_time;

    if ( !actor_action_allow_cover_seeking(actor_index, 0) )
        return 0;

    if ( actor_action_try_to_seek_cover(actor_index, 1u, 0) )
        return 1;

    if ( allow_panicking
      && actor_action_try_to_panic(actor_index, _actor_panic_no_cover_available, actor->target.target_prop_index, force_panicking) )
        return 1;

    return 0;
}
