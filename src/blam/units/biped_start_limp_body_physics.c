/* biped_start_limp_body_physics @0x837AD558 — switch a biped into limp-body (ragdoll-noodle) physics if its
 * biped definition allows it (biped definition flags bit 9 = "uses limp body physics") and the object
 * is connected to the map (object.flags bit 5, 0x20). No-ops when biped.flags already has bit 0
 * (_biped_airborne_bit) or bit 5 (_biped_limp_body_physics_active_bit) set. Otherwise: clears the
 * relaxation counter, latches the definition's max relaxation iterations, marks the object flags with
 * 0x800000 (limp-body physics), and sets the limp-body-active state bit. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/object_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/biped_datum_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t biped_limp_noodle_get_max_relaxation_iterations(int biped_index);

void biped_start_limp_body_physics(int biped_index)
{
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);

    biped_definition *definition = TAG_GET(biped_definition, biped->definition_index);
    if ( ((definition->biped.flags >> _biped_uses_limp_body_physics_bit) & 1) != 0
      && (biped->object.flags & (1u << _object_at_rest_bit)) != 0 )
    {
        unsigned int limp_state = biped->biped.flags;
        if ( (limp_state & (1u << _biped_airborne_bit)) == 0
          && (limp_state & (1u << _biped_limp_body_physics_active_bit)) == 0 )
        {
            biped->biped.limp_body_current_relaxation_iterations = 0;
            uint8_t max_relaxation_iterations = biped_limp_noodle_get_max_relaxation_iterations(biped_index);
            biped->biped.limp_body_max_relaxation_iterations = max_relaxation_iterations;
            biped->object.flags |= (1u << _object_do_not_recompute_node_matrices_bit);
            biped->biped.flags |= (1u << _biped_limp_body_physics_active_bit);
        }
    }
}
