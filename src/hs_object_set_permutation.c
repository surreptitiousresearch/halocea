/* hs_object_set_permutation @0x837F8360 — set the active permutation of one (or all) of an object's model
 * regions. If region_name matches the "all regions" sentinel string, the permutation is applied to every
 * region (region index -1); otherwise the named region is looked up in the object's model tag (region names
 * are 76-byte records) and the permutation applied just to it.
 *
 * DEVIATION: the sentinel-string comparison is reproduced as the binary's inline character compare against
 * the global "all" string (empty_string). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/_object_definition.h"
#include "headers/model.h"
#include "headers/model_region.h"
#include "headers/blam_data_globals.h"

extern char empty_string[];   /* "all" — all-regions sentinel */
extern int stricmp(const char *a, const char *b);
extern void object_permute_region(int object_index, const char *permutation_name, int16_t desired_region_index, uint8_t active_flag);

void hs_object_set_permutation(int object_index, const char *region_name, char *permutation_name)
{
    if ( object_index == -1 )
        return;

    /* Inline compare of region_name against the all-regions sentinel. */
    const char *a = region_name;
    char *b = empty_string;
    int diff;
    do
    {
        int c = *(unsigned char *)a;
        diff = c - *(unsigned char *)b;
        if ( c == 0 )
            break;
        ++a;
        ++b;
    }
    while ( !diff );

    int16_t region_index = -1;
    if ( diff )
    {
        object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        int object_tag_index = object->definition_index;
        int model_tag_index = TAG_GET(_object_definition, object_tag_index)->model.index;
        if ( model_tag_index != -1 )
        {
            model *model_data = TAG_GET(model, model_tag_index);
            int region_count = model_data->regions.count;
            if ( region_count > 0 )
            {
                int i = 0;
                while ( stricmp(((model_region *)model_data->regions.address)[i].name, region_name) )
                {
                    i = (int16_t)(i + 1);
                    if ( i >= model_data->regions.count )
                        goto apply;
                }
                region_index = i;
            }
        }
    }

apply:
    object_permute_region(object_index, permutation_name, region_index, 1u);
}
