/* biped_update_dead @0x837B28C0 — per-frame animation update for a dead biped. While the corpse is
 * still settling (limp flag 0x20 set and the relax counter hasn't reached its limit) it relaxes the
 * ragdoll nodes onto the environment. Once settled it requests either the "dead" (24) or "dead settled"
 * (25) animation state depending on whether the body has come to rest, snapping its facing when it has. */

#include <stdint.h>
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/unit_animation_update_data.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/biped_definition_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/biped_datum_flags.h"
#include "headers/blam_data_globals.h"


extern int biped_limp_noodle_relax_nodes_onto_environment(int biped_index);
extern void biped_snap_facing(biped_datum *biped, const biped_definition *biped_definition);
extern void biped_apply_rotation(int biped_index);

void biped_update_dead(int biped_index, unit_animation_update_data *animation)
{
    /* object-header datum pool entry is 12 bytes (salted); the biped_datum pointer lives at dword +2 */
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);

    const biped_definition *definition =
        TAG_GET(const biped_definition, biped->definition_index);

    if ((biped->biped.flags & (1u << _biped_limp_body_physics_active_bit)) != 0
        && biped->biped.limp_body_current_relaxation_iterations
             < (unsigned int)biped->biped.limp_body_max_relaxation_iterations)
    {
        biped_limp_noodle_relax_nodes_onto_environment(biped_index);
        animation->crouching = 0;
    }
    else
    {
        char state_desired;
        if (biped->biped.airborne_ticks < 3 || (definition->biped.flags & (1u << _biped_has_no_dying_airborne_bit)) != 0)
        {
            if ((uint8_t)biped->unit.animation.state == _unit_state_dying_airborne)
            {
                biped->biped.bank = 0.0f;
                biped_snap_facing(biped, definition);
            }
            state_desired = _unit_state_dying;
        }
        else
        {
            if ((uint8_t)biped->unit.animation.state == _unit_state_dying_airborne)
                biped_apply_rotation(biped_index);
            state_desired = _unit_state_dying_airborne;
        }
        animation->state_desired = state_desired;
        animation->crouching = 0;
    }
}
