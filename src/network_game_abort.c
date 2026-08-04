#include "headers/blam_data_globals.h"
/* network_game_abort @ 0x837569F0 — request networking teardown, close chat */
extern void hud_chat_close(void);
void network_game_abort(void) /* attested void: tail-call of void fn, 0/4 callers consume r3 */
{
    want_to_teardown_networking = 1;
    hud_chat_close();
}
