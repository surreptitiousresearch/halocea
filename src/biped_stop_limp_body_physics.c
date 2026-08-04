/* biped_stop_limp_body_physics @0x837AD640 — stop ragdoll ("limp body") physics on a biped: if the
 * biped definition allows it (biped.flags bit 0x200), clear the biped's limp-body state bit
 * (biped.flags bit 0x20) and the matching object flag (object.flags bit 0x00800000). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/object_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/biped_datum_flags.h"
#include "headers/blam_data_globals.h"


void biped_stop_limp_body_physics(int biped_index)
{
    biped_datum *biped =
        (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;
    biped_definition *def = TAG_GET(biped_definition, biped->definition_index);
    if ( (def->biped.flags & (1u << _biped_uses_limp_body_physics_bit)) != 0 )
    {
        if ( (biped->biped.flags & (1u << _biped_limp_body_physics_active_bit)) != 0 )
        {
            biped->biped.flags &= ~(1u << _biped_limp_body_physics_active_bit);
            biped->object.flags &= ~(1u << _object_do_not_recompute_node_matrices_bit);
        }
    }
}
