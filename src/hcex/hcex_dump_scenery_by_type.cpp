/* hcex_dump_scenery_by_type @0x836827A0 — HCEX debug dump: for every scenario object placement of
 * `type`, resolve its palette entry to a tag definition and, if the tag has a model path, forward it (with
 * placement index/type/name, world position, and forward/up basis derived from its rotation) to
 * hcex_scenery_output — skipping anything under "weapons\" (unless `type` is 1 and the model name contains
 * "chair" or "cryotube", i.e. seat-like placements are still reported for the vehicle type class).
 * `type` is an object_type enum value (DB $F5AF3705676D16CDF71575CA63411CB1).
 *
 * Field offset 44 on the resolved tag definition (model path) and the palette-entry-to-tag-definition
 * lookup (`palette->address + 12*index + 3`) reuse the established idioms from object_new_from_scenario.c
 * / object_types_place_objects.c. */

#include <stdint.h>
#include "../headers/scenario.h"
#include "../headers/scenario_object_datum.h"
#include "../headers/tag_block.h"
#include "../headers/global_tag_instances.h"
#include "../headers/hcex/hcex_float3.h"
#include "../headers/real_vector3d.h"
#include "../headers/object_type.h"

#include "headers/scenario.h"
#include "../headers/scenario_object_palette_entry.h"
extern "C" scenario *global_scenario;

extern "C" tag_block *scenario_get_object_type_scenario_datums(scenario *scenario, int16_t object_type, int *size);
extern "C" tag_block * scenario_get_object_type_scenario_palette(scenario *scenario, int16_t object_type);
extern "C" char *tag_block_get_element_with_size(const tag_block *block, int index, int element_size);
extern "C" void vectors3d_from_euler_angles3d(real_vector3d *forward, real_vector3d *up, const real_euler_angles3d *angles);
extern "C" void hcex_scenery_output(int obj_id, int obj_type, const char *model_name, hcex_float3 *pos, hcex_float3 *fw,
    hcex_float3 *up);
extern "C" char *strstr(const char *str1, const char *str2);

extern "C" void hcex_dump_scenery_by_type(int type)
{
    int element_size;
    const tag_block *scenario_datums = scenario_get_object_type_scenario_datums(global_scenario, type, &element_size);
    tag_block *scenario_palette = scenario_get_object_type_scenario_palette(global_scenario, type);

    for ( int i = 0; i < scenario_datums->count; i = (int16_t)(i + 1) )
    {
        scenario_object_datum *placement =
            (scenario_object_datum *)tag_block_get_element_with_size(scenario_datums, i, element_size);

        if ( placement->palette_entry_index == -1 )
            continue;

        /* the folded int slot `12*idx + 3` is byte 48*idx + 12, i.e.
         * sizeof(scenario_object_palette_entry) with tag_reference.index 12 bytes in */
        const scenario_object_palette_entry *palette =
            (const scenario_object_palette_entry *)scenario_palette->address;
        int tag_index = palette[placement->palette_entry_index].reference.index;
        int tag_definition = *(int *)TAG_INSTANCE(tag_index);

        hcex_float3 position;
        position.x = placement->position.n[0];
        position.y = placement->position.n[1];
        position.z = placement->position.n[2];

        hcex_float3 forward;
        real_vector3d up;
        vectors3d_from_euler_angles3d((real_vector3d *)&forward, &up, &placement->rotation);

        const char *model_name = *(const char **)(tag_definition + 44);
        if ( model_name
          && !strstr(model_name, "weapons\\")
          && (type != object_type_vehicle || strstr(model_name, "chair") || strstr(model_name, "cryotube")) )
        {
            hcex_scenery_output(i, type, model_name, &position, &forward, (hcex_float3 *)&up);
        }
    }
}
