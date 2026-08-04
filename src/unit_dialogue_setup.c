/* unit_dialogue_setup @0x837BB34C — resolve which dialogue variant a unit should use: tries its requested
 * variant number (+190) if positive, falling back to variant 0, then variant -1 ("any") if each attempt
 * fails to find a match. Stores the resolved variant index at object+900.
 *
 * DEVIATION: the decompiler introduced an extra intermediate pointer variable for the tag lookup; this is
 * exactly the standard TAG_INSTANCE(object tag index) -> unit_definition* pattern (dword offset 5 = byte 20
 * = cache_file_tag_instance.base_address), just rendered with one more hop than usual. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_definition.h"
#include "headers/blam_data_globals.h"


extern int unit_find_dialogue_variant(const unit_definition *definition, int16_t variant_number);

void unit_dialogue_setup(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    __int16 requested_variant = unit->object.variant_number;
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);

    int dialogue_variant = -1;
    if ( requested_variant > 0 )
        dialogue_variant = unit_find_dialogue_variant(definition, requested_variant);

    if ( dialogue_variant == -1 )
    {
        dialogue_variant = unit_find_dialogue_variant(definition, 0);
        if ( dialogue_variant == -1 )
            dialogue_variant = unit_find_dialogue_variant(definition, -1);
    }

    unit->unit.dialogue_index = dialogue_variant;
}
