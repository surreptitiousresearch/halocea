/* unit_destroy @0x836C9098 — destroy a unit object: tear down the generic object, then run the unit's
 * spawn-point bookkeeping (unit_test_spawning) so any respawn logic notices the loss. */

extern void object_destroy(int object_index);
extern int unit_test_spawning(int unit_index);

void unit_destroy(int unit_index)
{
    object_destroy(unit_index);
    unit_test_spawning(unit_index);
}
