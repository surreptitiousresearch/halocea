/* get_material_type @0x837BE098 — resolve a material index to its damage_material.type (see
 * damage_material.h / damage_resistance.h). With no object (object_index == -1), looks up the current
 * structure BSP's own 20-byte collision-material records instead. -1 for an invalid material_index.
 *
 * DEVIATION: hand-traced the nested TAG_INSTANCE chain (object tag index at object+0, standard Blam
 * convention -> tag-authored object definition -> a damage_resistance tag index at definition+124 ->
 * TAG_INSTANCE again -> damage_resistance) rather than trusting the decompiler's raw nested-dereference
 * rendering; confirmed the final +72*material_index+36 matches damage_material's own type field exactly
 * (see object_cause_damage.c's damage_material usage), and that damage_resistance's materials.address tag_block
 * field sits at +568 (0x234 tag_block start + 4), matching the decompiler's dword offset 142. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/structure_bsp.h"
#include "headers/structure_collision_material.h"
#include "headers/damage_resistance.h"
#include "headers/damage_material.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/blam_data_globals.h"


int16_t get_material_type(int object_index, int16_t material_index)
{
    if ( material_index == -1 )
        return -1;

    if ( object_index == -1 )
        return ((structure_collision_material *)global_structure_bsp->collision_materials.address)[material_index].runtime_physics_material_type;

    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *object_def = TAG_GET(_object_definition, object->definition_index);
    int resistance_tag_index = object_def->collision_model.index; /* definition +124 — the 'coll' tag carries the damage_resistance body */
    const damage_resistance *resistance = TAG_GET(const damage_resistance, resistance_tag_index);

    return ((damage_material *)resistance->materials.address)[material_index].type;
}
