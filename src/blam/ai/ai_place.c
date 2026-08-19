/* ai_place @0x836E7700 — tail-call thunk to encounters_create_for_new_map. */

/* encounters_create_for_new_map returns void (residue + 0-consumer, wave 28). */
extern void encounters_create_for_new_map(void);

/* Pure tail-thunk: `b encounters_create_for_new_map`. Callee is void and ai_place's sole
 * caller (game_initialize_for_new_map) ignores r3, so ai_place is void. */
void ai_place(void)
{
    encounters_create_for_new_map();
}
