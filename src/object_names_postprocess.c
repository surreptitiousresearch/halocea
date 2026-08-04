/* object_names_postprocess @0x83703130 — (non-editing scenarios only) stamp each named object placement
 * with its owning object-type index and placement index within that type's palette: for each of the 12
 * object types that has both a placement and palette tag block, walk its placement array and, for every
 * placement whose name index (word +1) is valid, write the object type (word +16) and placement index
 * (word +17) into the corresponding scenario.object_names entry (36-byte stride). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_object_name.h"
#include "headers/object_type_definition.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern char *tag_block_get_element_with_size(const tag_block *block, int index, int element_size);

void object_names_postprocess(scenario *scenario, uint8_t editing)
{
    if ( editing )
        return;

    for ( __int16 object_type = object_type_biped; object_type < number_of_object_types; object_type++ )
    {
        object_type_definition *type_definition = object_type_definitions[object_type];

        if ( (unsigned __int16)type_definition->placement_tag_block_offset == 0xFFFF
          || (unsigned __int16)type_definition->palette_tag_block_offset == 0xFFFF )
            continue;

        int element_size = type_definition->placement_tag_block_element_size;
        const tag_block *placements = (const tag_block *)((char *)scenario + type_definition->placement_tag_block_offset);

        for ( __int16 placement_index = 0; placement_index < placements->count; placement_index++ )
        {
            __int16 name_index = *((__int16 *)tag_block_get_element_with_size(placements, placement_index, element_size) + 1);
            if ( name_index != -1 )
            {
                scenario_object_name *object_name = &((scenario_object_name *)scenario->object_names.address)[name_index];
                object_name->runtime_object_type = object_type;
                object_name->runtime_scenario_datum_index = placement_index;
            }
        }
    }
}
