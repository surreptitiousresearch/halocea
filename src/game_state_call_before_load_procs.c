/* game_state_call_before_load_procs @0x836844E0 — prepare for a game-state load: end any film recording, then
 * invoke the first registered before-load proc.
 *
 * Deviation: save_film_end_recording's return feeding before_load_procs[0] is ABI noise (the proc takes no
 * meaningful argument); reproduced as independent calls. */

extern void save_film_end_recording(void);
extern void (*before_load_procs[])(void);

void game_state_call_before_load_procs(void)
{
    save_film_end_recording();
    before_load_procs[0]();
}
