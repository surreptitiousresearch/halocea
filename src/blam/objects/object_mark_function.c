/* object_mark_function @0x836EDCC8 — mark an object for the current marker generation. Returns 0 if it was
 * already marked, else stamps the marker word (object_data+20) and returns 1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


/* DEVIATION (2026-08-12, #134): object_index is `int`, restoring the spelling a 2026-07-30 pass
 * changed. That note read "disasm first-use clrlwi r10,r3,16; matches its three visibility-callback
 * siblings" — the mask is DATA_ARRAY_ELEMENT's own truncation (data_array.h), and the three siblings
 * had been narrowed by citing THIS function. The caller settles it: structure_visibility_find_objects
 * @0x837C4DA8 stores the walker's index whole into `int *result_indices` (`stwx r31, r11, r25`
 * @0x837C4E98) and passes that same r31 to both mark callbacks (`mr r3, r31 / bctrl` @0x837C4E90-9C)
 * and to get_bounding_sphere(int, ...). The pregrade graded this row UNRESOLVABLE because "its width
 * is fixed by the callback slot in find_rendered_objects, which no evidence in this pass reaches" —
 * that slot is read here, so the refusal is lifted on evidence rather than on convention. */
int object_mark_function(int object_index)
{
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    if (object_data->object.magic_number == global_object_marker)
        return 0;
    object_data->object.magic_number = global_object_marker;
    return 1;
}
