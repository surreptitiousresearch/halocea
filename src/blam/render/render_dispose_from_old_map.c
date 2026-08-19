/* render_dispose_from_old_map @ 0x83707190 — thunk to render_objects_dispose_from_old_map */
/* attested: void return — tail call to void callee; sole caller (game_dispose_from_old_map) ignores r3 */
extern void render_objects_dispose_from_old_map(void);
void render_dispose_from_old_map(void)
{
    render_objects_dispose_from_old_map();
}
