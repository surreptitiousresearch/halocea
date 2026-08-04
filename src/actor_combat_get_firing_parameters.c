/* actor_combat_get_firing_parameters @0x837B7EC0 — select the burst geometry and firing pattern an
 * actor should use this combat frame, based on its state flags (emotions.berserk, control.firing_at_new_target,
 * control.firing_while_moving). The burst geometry is always the variant's ranged-combat geometry;
 * the firing pattern is chosen by state (null when idle). */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_burst_geometry.h"
#include "headers/actor_firing_pattern.h"
#include "headers/blam_data_globals.h"


/* actor_index attested int: callers pass the full datum handle (mr r3,r29 @837B8D24);
 * clrlwi 16 in the body is the index extraction, not a 16-bit param */
void actor_combat_get_firing_parameters(int actor_index, actor_variant_definition *firing_variant_definition,
                                        actor_burst_geometry **burst_geometry_reference, actor_firing_pattern **firing_pattern_reference)
{
    actor_burst_geometry *burst_geometry = &firing_variant_definition->ranged_combat.burst_geometry;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->emotions.berserk )           /* berserk */
    {
        *burst_geometry_reference = burst_geometry;
        *firing_pattern_reference = &firing_variant_definition->ranged_combat.berserk_pattern;
    }
    else if ( actor->control.firing_at_new_target )     /* new target */
    {
        *burst_geometry_reference = burst_geometry;
        *firing_pattern_reference = &firing_variant_definition->ranged_combat.new_target_pattern;
    }
    else
    {
        *burst_geometry_reference = burst_geometry;
        if ( actor->control.firing_while_moving )      /* moving */
            *firing_pattern_reference = &firing_variant_definition->ranged_combat.moving_pattern;
        else
            *firing_pattern_reference = 0;
    }
}
