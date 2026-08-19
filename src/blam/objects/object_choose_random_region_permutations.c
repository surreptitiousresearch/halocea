/* object_choose_random_region_permutations @0x836ED468 — pick the per-region permutation set for a new
 * object from its model tag, honouring a requested variant first and otherwise falling back to the
 * model's default variant (the variant number of the first region's already-chosen permutation). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/model.h"
#include "headers/model_region.h"
#include "headers/model_region_permutation.h"
#include "headers/blam_data_globals.h"

extern uint8_t object_select_random_region_permutations_by_variant(int object_index, const model *model, int16_t variant_number);

extern int object_determine_variant_number(int object_index, model *model);
void object_choose_random_region_permutations(int object_index)
{
    object_header_datum *header =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object = header->datum;

    int model_index =
        TAG_GET(_object_definition, object->definition_index)->model.index;
    if ( model_index == -1 )
        return;

    int16_t requested_variant = object->object.variant_number;
    unsigned char matched = 0;
    model *model_tag = TAG_GET(model, model_index);

    if ( requested_variant > 0 )
        matched = object_select_random_region_permutations_by_variant(object_index, model_tag, requested_variant);

    if ( !matched )
    {
        object_select_random_region_permutations_by_variant(object_index, model_tag, -1);

        /* DEVIATION: collapsed verbatim-inlined body of object_determine_variant_number@0x836ED3D8 (zero-xref donor) back into a call; region-walk/variant_number field-reads matched donor exactly at disasm grade, no arg folding required. */
        int default_variant = object_determine_variant_number(object_index, model_tag);

        object->object.variant_number = default_variant;
        if ( default_variant > 0 )
            object_select_random_region_permutations_by_variant(object_index, model_tag, default_variant);
    }
}
