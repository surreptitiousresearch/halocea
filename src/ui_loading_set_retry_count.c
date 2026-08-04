#include "headers/blam_data_globals.h"
/* ui_loading_set_retry_count @0x83764448 — setter for the loading-screen retry counter (see
 * ui_loading_init_state.c for the same global). */


void ui_loading_set_retry_count(int count)
{
    loading_screen_retry_count = count;
}
