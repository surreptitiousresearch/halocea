/* scenario_get_structure_reference_index_from_tag_index @0x83703CA0 — find the index of `structure_bsp_index`'s
 * tag name within the scenario's structure_bsp_references list (matched by comparing the tag's name string
 * against each reference's structure_bsp.name), or -1 if not present.
 *
 * DEVIATION: the decompiler rendered the name comparison as a manual byte-by-byte loop; this is exactly what
 * an inlined strcmp expands to, restored as such. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_structure_bsp_reference.h"

extern char *tag_get_name(int tag_index);
extern int strcmp(const char *a, const char *b);

int16_t scenario_get_structure_reference_index_from_tag_index(scenario *scenario, int structure_bsp_index)
{
    const char *name = tag_get_name(structure_bsp_index);
    int count = scenario->structure_bsp_references.count;
    scenario_structure_bsp_reference *references = (scenario_structure_bsp_reference *)scenario->structure_bsp_references.address;

    for ( int i = 0; i < count; ++i )
    {
        if ( !strcmp(name, references[i].structure_bsp.name) )
            return i;
    }

    return -1;
}
