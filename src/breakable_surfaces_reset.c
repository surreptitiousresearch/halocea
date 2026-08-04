/* breakable_surfaces_reset @0x83743100 — thunk: re-run the per-map breakable-surface initialization,
 * restoring every destructible surface to its undamaged state. */

extern void breakable_surfaces_initialize_for_new_map(void);

void breakable_surfaces_reset(void)
{
    breakable_surfaces_initialize_for_new_map(); return;
}
