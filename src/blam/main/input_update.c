/* input_update @ 0x836FBD98 — thunk to input_update_keyboard */
extern unsigned int input_update_keyboard(void);
void input_update(void) /* attested void: tail-call thunk, 0/3 callers consume r3 */
{
    input_update_keyboard();
}
