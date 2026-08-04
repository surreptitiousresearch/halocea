/* render_dispose @ 0x83707198 — thunk to render_objects_dispose */

extern void render_objects_dispose(void);

void render_dispose(void) /* attested void: tail-call thunk of void fn, 0/1 callers consume r3 */
{
    render_objects_dispose();
}
