#include "headers/blam_data_globals.h"
/* ui_loading_set_cookie @0x837644A8 — setter for the loading-screen NAT negotiation cookie (see
 * ui_loading_init_state.c for the same global). */


void ui_loading_set_cookie(int negotiate_cookie)
{
    loading_screen_natneg_cookie = negotiate_cookie;
}
