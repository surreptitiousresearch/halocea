/* render_initialize @ 0x83707180 — thunk to render_objects_initialize */

extern void render_objects_initialize(void);

void render_initialize(void)
{
    render_objects_initialize(); /* attested void: r3-thread flattened */
}
