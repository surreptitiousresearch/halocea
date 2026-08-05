/* object_new_from_scenario @0x836F2248 — instantiate one scenario-placed object from its palette entry and
 * placement datum, then run its per-type placement and register its scenario name. Returns the new object
 * index, or -1 if the palette entry is empty, the placement is suppressed during initial placement, the name
 * is already bound, or object creation fails. */

#include <stdint.h>
#include "headers/scenario_object_datum.h"
#include "headers/scenario_object_palette_entry.h"
#include "headers/object_header_datum.h"
#include "headers/object_placement_data.h"
#include "headers/object_globals.h"
#include "headers/tag_block.h"
#include "headers/data_array.h"
#include "headers/scenario_object_location_placement_flags.h"
#include "headers/scenario_constants.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
#include "headers/real_euler_angles3d.h"
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern void vectors3d_from_euler_angles3d(real_vector3d *forward, real_vector3d *up, const real_euler_angles3d *angles);
extern int object_new(object_placement_data *data);
extern void object_type_place(int object_index, scenario_object_datum *scenario_object);

int object_new_from_scenario(scenario_object_datum *scenario_object, tag_block *palette)
{
    int16_t palette_entry_index = scenario_object->palette_entry_index;
    int result = -1;
    if ( palette_entry_index != -1
      && (!object_globals->initial_placement || (scenario_object->placement_flags & (1u << _scenario_object_placement_not_automatic_bit)) == 0) )
    {
        int name_index = (uint16_t)scenario_object->name_index;
        if ( name_index == 0xFFFF
          || (unsigned int)(int16_t)name_index >= MAXIMUM_OBJECT_NAMES_PER_SCENARIO
          || object_name_list[(int16_t)name_index] == -1 )
        {
            int object_definition_index =
                ((scenario_object_palette_entry *)palette->address)[palette_entry_index].reference.index;
            if ( object_definition_index != -1 )
            {
                object_placement_data placement;
                object_placement_data_new(&placement, object_definition_index, -1);
                placement.position.n[0] = scenario_object->position.n[0];
                placement.position.n[1] = scenario_object->position.n[1];
                placement.position.n[2] = scenario_object->position.n[2];
                vectors3d_from_euler_angles3d(&placement.forward, &placement.up, &scenario_object->rotation);
                placement.variant_number = scenario_object->variant_number;
                result = object_new(&placement);
                if ( result != -1 )
                {
                    object_type_place(result, scenario_object);
                    int name = (uint16_t)scenario_object->name_index;
                    if ( name != 0xFFFF )
                    {
                        int name_slot = (int16_t)name;
                        object_datum *object_data =
                            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, result)->datum;
                        if ( object_name_list[name_slot] == -1 )
                        {
                            object_name_list[name_slot] = result;
                            object_data->object.name_index = name;
                        }
                    }
                }
            }
        }
    }
    return result;
}
