/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* object_type_synchronize @0x83701998 — reconcile a live object with its scenario placement record when the
 * scenario is (re)synchronized. If the placement references a palette entry: an existing object of the same
 * definition is reused, one of a different definition is deleted and recreated, and none is created fresh. The
 * (re)created/reused object is activated and positioned from the placement's position and Euler rotation — with
 * the Z raised by half the object definition's "height" field unless that definition opts out. If the placement
 * has no palette entry, any existing object is deleted. Finally the scenario name table is pointed at the
 * resulting object. Returns the object index (or -1).
 *
 * The goto-based control flow is reflowed into structured conditionals. */

#include <stdint.h>
#include "headers/scenario_object_datum.h"
#include "headers/scenario_object_name.h"
#include "headers/scenario_object_palette_entry.h"
#include "headers/tag_block.h"
#include "headers/object_placement_data.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/_object_definition.h"
#include "headers/tag_reference.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
#include "headers/real_euler_angles3d.h"
#include "headers/real_vector3d.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void object_delete(int object_index);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern void vectors3d_from_euler_angles3d(real_vector3d *forward, real_vector3d *up, const real_euler_angles3d *angles);
extern int object_new(object_placement_data *data);
extern void object_type_place(int object_index, scenario_object_datum *scenario_object);
extern void object_activate(int object_index);
extern void matrix4x3_rotation_from_angles(real_matrix4x3 *matrix, float yaw, float pitch, float roll);
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);
extern void object_set_object_index_for_name_index(int16_t name_index, int object_index);

int object_type_synchronize(int object_index, scenario_object_datum *scenario_object, tag_block *palette, int16_t object_type, int16_t scenario_datum_index)
{
    int result_object = object_index;

    if ( scenario_object->palette_entry_index != -1 )
    {
        /* the folded 48 was sizeof(scenario_object_palette_entry) */
        scenario_object_palette_entry *palette_entry =
            &((scenario_object_palette_entry *)palette->address)[scenario_object->palette_entry_index];
        int definition_index = -1;
        int need_new = 1;

        if ( object_index == -1 )
        {
            definition_index = palette_entry->reference.index;
        }
        else
        {
            int *existing = object_try_and_get_and_verify_type(object_index, object_mask_all);
            if ( existing && *existing == palette_entry->reference.index )
            {
                need_new = 0;
            }
            else
            {
                if ( existing )
                    object_delete(result_object);
                definition_index = palette_entry->reference.index;
                result_object = -1;
            }
        }

        int proceed = 1;
        if ( need_new )
        {
            if ( definition_index == -1 )
            {
                proceed = 0;
            }
            else
            {
                object_placement_data placement;
                object_placement_data_new(&placement, definition_index, -1);
                placement.position = scenario_object->position;
                vectors3d_from_euler_angles3d(&placement.forward, &placement.up, &scenario_object->rotation);
                placement.variant_number = scenario_object->variant_number;

                result_object = object_new(&placement);
                if ( result_object == -1 )
                    proceed = 0;
                else
                    object_type_place(result_object, scenario_object);
            }
        }

        if ( proceed )
        {
            object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, result_object)->datum;
            object_activate(result_object);

            real_matrix4x3 orientation;
            matrix4x3_rotation_from_angles(&orientation, scenario_object->rotation.n[0],
                    scenario_object->rotation.n[1], scenario_object->rotation.n[2]);

            real_point3d adjusted;
            const real_point3d *p_position;
            _object_definition *definition = TAG_GET(_object_definition, object->definition_index);
            if ( definition->physics.index == -1 )
            {
                p_position = &scenario_object->position;
            }
            else
            {
                adjusted.n[0] = scenario_object->position.n[0];
                adjusted.n[1] = scenario_object->position.n[1];
                adjusted.n[2] = scenario_object->position.n[2] + object->object.bounding_sphere_radius * 0.5f;
                p_position = &adjusted;
            }
            object_set_position(result_object, p_position, &orientation.___u1.__s1.forward, &orientation.___u1.__s1.up);
            object->object.name_index = scenario_object->name_index;
        }
    }
    else if ( object_index != -1 )
    {
        object_delete(object_index);
        result_object = -1;
    }

    if ( (uint16_t)scenario_object->name_index != 0xFFFF )
    {
        scenario_object_name *name_entry = (scenario_object_name *)global_scenario->object_names.address
                + scenario_object->name_index;
        name_entry->runtime_object_type = object_type;
        name_entry->runtime_scenario_datum_index = scenario_datum_index;
        object_set_object_index_for_name_index(scenario_object->name_index, result_object);
    }
    return result_object;
}
