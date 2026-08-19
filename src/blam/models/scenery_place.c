/* scenery_place @0x837E9D58 — tail-call forwarder (`addi r4, r4, 0x28; b object_add_scenario_permutation`;
 * confirmed via disasm, not a bare blr). scenario_scenery_datum embeds a scenario_object_permutation at
 * +0x28; this wrapper simply offsets the pointer to that embedded struct and forwards. Neither type's full
 * layout is needed here since only pointer arithmetic occurs — both left opaque.
 * object_add_scenario_permutation itself is not yet decompiled — declared extern with its own DB
 * prototype. */

#include "headers/scenario_scenery_datum.h"
#include "headers/scenario_object_permutation.h"

extern void object_add_scenario_permutation(int object_index, struct scenario_object_permutation *permutation);

void scenery_place(int scenery_index, scenario_scenery_datum *scenario_scenery)
{
    /* recovered: (char *)scenario_scenery + 0x28 -> &scenario_scenery->permutation */
    object_add_scenario_permutation(scenery_index,
            &scenario_scenery->permutation);
}
