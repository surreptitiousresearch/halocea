/* render_initialize_for_new_map @0x83707188 — thunk to render_objects_initialize_for_new_map. */

extern void render_objects_initialize_for_new_map(void);

void render_initialize_for_new_map(void)
{
    render_objects_initialize_for_new_map(); /* attested void callee: r3-thread removed */
}
