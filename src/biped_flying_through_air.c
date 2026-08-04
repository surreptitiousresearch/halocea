/* biped_flying_through_air @0x837AEA30 — true when a biped is airborne: its airborne tick counter
 * (biped.airborne_ticks) is past the grounded threshold (> 3), and either the biped definition does not
 * fly (_biped_flying_bit clear) or the object is dead (_object_dead_bit set). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/object_damage_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/blam_data_globals.h"


uint8_t biped_flying_through_air(int biped_index)  /* BOOL->int */
{
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);
    biped_definition *definition = TAG_GET(biped_definition, biped->definition_index);
    return biped->biped.airborne_ticks > 3
        && ((definition->biped.flags & (1u << _biped_flying_bit)) == 0 || (biped->object.damage_flags & (1u << _object_dead_bit)) != 0);
}
