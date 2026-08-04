/* object_add_scenario_permutation @0x836EC6D8 — empty in this build (a single blr). The scenario-object
 * permutation registration hook compiles to a no-op here; reproduced faithfully. */

struct scenario_object_permutation;

void object_add_scenario_permutation(int object_index, struct scenario_object_permutation *permutation)
{
    (void)object_index;
    (void)permutation;
}
