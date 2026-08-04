/* unit_dialogue_determine_variant @0x837BA9A0 — pick a random dialogue variant for a unit that doesn't
 * already have one (unit +190 word, the same "dialogue_variant" field ai_conversation_begin.c reads/writes).
 * Scans the unit definition's dialogue-variant table (definition +692 count, +696 array, 24-byte records,
 * candidate weight word at +0 of each record) for up to 16 candidates whose weight is below 100, then
 * round-robins through the collected candidates via a global sequential counter (rather than randomly). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/dialogue_variant_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


void unit_dialogue_determine_variant(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);

    if ( unit->object.variant_number )
        return;   /* already has a dialogue variant */

    int candidate_count = definition->unit.dialogue_variants.count;
    __int16 candidates[16];
    unsigned __int16 collected = 0;

    if ( candidate_count > 0 )
    {
        dialogue_variant_definition *records = (dialogue_variant_definition *)definition->unit.dialogue_variants.address;
        for ( int i = 0; i < candidate_count; i++ )
        {
            __int16 variant_number = records[i].variant_number;
            if ( variant_number < 100 )
            {
                if ( collected >= 16 )
                    break;
                candidates[collected++] = variant_number;
            }
        }
    }

    if ( (__int16)collected > 0 )
    {
        __int16 chosen = candidates[sequential_counter % (__int16)collected];
        ++sequential_counter;
        unit->object.variant_number = chosen;
    }
}
