#include "headers/blam_data_globals.h"
/* ui_loading_init_state @ 0x837644B8 — reset the loading-screen state machine */

void ui_loading_init_state(void)
{
    loading_screen_close_time_ms = -1;
    loading_screen_open_time_ms = -1;
    loading_screen_retry_count = 0;
    loading_screen_state = 0;
    loading_screen_connect_ip[0] = 0;
    loading_screen_map[0] = 0;
    loading_screen_natneg_cookie = -1;
}
