#include "headers/blam_data_globals.h"
extern void hud_chat_close(void);

void network_game_client_all_local_players_have_quit(void) /* attested void: tail-call of void fn, funcptr-reached */
{
    want_to_teardown_networking = 1;
    hud_chat_close();
}
