/* object_permutation_shield_regions @ 0x836B24D0 — enable or disable the "shielded" permutation on the
 * object's collision-model regions when its shield comes back up or goes down. Walks the collision
 * model's damage_resistance.regions; for each region that both is flagged as a shield region
 * (flags & 0x10) and has more than one permutation, it writes the object's per-region
 * region_permutations byte. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/collision_model.h"
#include "headers/damage_region.h"
#include "headers/object_region_flags.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


void object_permutation_shield_regions(int object_index, uint8_t active)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *definition =
        TAG_GET(_object_definition, object->definition_index);
    collision_model *collision = TAG_GET(collision_model, definition->collision_model.index);

    if ( collision->resistance.regions.count > 0 )
    {
        __int16 i = 0;
        do
        {
            damage_region *region = &((damage_region *)collision->resistance.regions.address)[i];
            if ( (region->flags & (1u << _object_region_missing_when_shield_is_zero_bit)) != 0 && region->permutations.count > 1 )
                /* cntlzw(active)&0x20 is non-zero exactly when active == 0 */
                object->object.region_permutations[i] = (active == 0);
            i = (__int16)(i + 1);
        }
        while ( i < collision->resistance.regions.count );
    }
}
