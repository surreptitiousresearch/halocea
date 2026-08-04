/* object_types_place_objects @0x83702C10 — for the object types skipped by object_types_place_all
 * (object_mask_remove_on_bsp_switch: scenery type 6 and light_fixture type 9), compute per-placement
 * structure-BSP membership for the current BSP, then (when `place` is set)
 * instantiate the placements that belong to this BSP. Each BSP is processed only once (processed_bsp_flags). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_object_datum.h"
#include "headers/object_type_definition.h"
#include "headers/tag_block.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/object_type.h"
#include "headers/scenario_object_palette_entry.h"
#include "headers/_object_definition.h"
#include "headers/scenario_object_location_placement_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_in_editor(void);
extern char *tag_block_get_element_with_size(const tag_block *block, int index, int element_size);
extern void matrix4x3_rotation_from_angles(real_matrix4x3 *matrix, float yaw, float pitch, float roll);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern int scenario_leaf_index_from_point(const real_point3d *point);
extern void objects_memory_compact(void);
extern int object_index_from_name_index(int16_t name_index);
extern int object_new_from_scenario(scenario_object_datum *scenario_object, tag_block *palette);
extern void objects_garbage_collection(void); /* decompiler shows a spurious arg; real signature is void(void) */

void object_types_place_objects(uint8_t place)
{
    if ( game_in_editor() || (unsigned __int16)global_structure_bsp_index == 0xFFFF )
        return;

    scenario *scenario_ptr = global_scenario;
    for ( int object_type = object_type_biped; object_type < number_of_object_types; object_type = (__int16)(object_type + 1) )
    {
        if ( ((1 << object_type) & object_mask_remove_on_bsp_switch) == 0 )
            continue;

        object_type_definition *type = object_type_definitions[object_type];
        if ( (unsigned __int16)type->placement_tag_block_offset == 0xFFFF
          || (unsigned __int16)type->palette_tag_block_offset == 0xFFFF )
            continue;

        int placement_element_size = type->placement_tag_block_element_size;
        const tag_block *placements = (const tag_block *)((char *)scenario_ptr + type->placement_tag_block_offset);
        tag_block *palette = (tag_block *)((char *)scenario_ptr + type->palette_tag_block_offset);

        /* compute BSP membership once for this BSP */
        if ( ((1 << global_structure_bsp_index) & processed_bsp_flags) == 0 && placements->count > 0 )
        {
            for ( int i = 0; i < placements->count; i = (__int16)(i + 1) )
            {
                scenario_object_datum *element = (scenario_object_datum *)tag_block_get_element_with_size(placements, i, placement_element_size);
                int palette_index = element->palette_entry_index; /* recovered: *element -> palette_entry_index */
                if ( palette_index == -1 )
                    continue;

                int object_definition_index =
                    ((scenario_object_palette_entry *)palette->address)[palette_index].reference.index;
                /* DB: _object_definition.bounding_offset (real_point3d) @ +8 — was raw char* + 8 */
                _object_definition *object_definition =
                    TAG_GET(_object_definition, object_definition_index);

                real_matrix4x3 placement_matrix;
                /* recovered: (float*)element+5..7 -> rotation.n[], +2..4 -> position.n[] */
                matrix4x3_rotation_from_angles(&placement_matrix,
                                               element->rotation.n[0],
                                               element->rotation.n[1],
                                               element->rotation.n[2]);
                placement_matrix.___u1.n[3][0] = element->position.n[0];
                placement_matrix.___u1.n[3][1] = element->position.n[1];
                placement_matrix.___u1.n[3][2] = element->position.n[2];

                real_point3d transformed;
                matrix4x3_transform_point(&placement_matrix, &object_definition->bounding_offset,
                                          &transformed);

                /* recovered: (element+4) -> &position, element[16] -> on_bsp_flags */
                if ( scenario_leaf_index_from_point(&element->position) == -1
                  && scenario_leaf_index_from_point(&transformed) == -1 )
                    element->on_bsp_flags &= ~(1 << global_structure_bsp_index);
                else
                    element->on_bsp_flags |= 1 << global_structure_bsp_index;
            }
        }

        /* place the objects belonging to this BSP */
        if ( place )
        {
            objects_memory_compact();
            for ( int i = 0; i < placements->count; i = (__int16)(i + 1) )
            {
                scenario_object_datum *datum =
                    (scenario_object_datum *)tag_block_get_element_with_size(placements, i, placement_element_size);
                int name_index = (unsigned __int16)datum->name_index;
                if ( (name_index == 0xFFFF || object_index_from_name_index(name_index) == -1)
                  && (datum->placement_flags & (1u << _scenario_object_placement_not_automatic_bit)) == 0
                  && ((1 << global_structure_bsp_index) & datum->on_bsp_flags) != 0 )
                {
                    global_object_scenario_index = i + 1;
                    object_new_from_scenario(datum, palette);
                    global_object_scenario_index = 0;
                    objects_garbage_collection();
                }
            }
        }
    }
    processed_bsp_flags |= 1 << global_structure_bsp_index;
}
