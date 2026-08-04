/* main_connect_private @0x8368AF18 — the client-connect path is compiled out in this build: it initializes
 * the loading screen, queues display error 53, and forces a return to the main menu, clearing the connect
 * request state.
 *
 * Deviation: ascii_to_wide writes a 16-wchar scratch buffer that is otherwise unused here (the converted
 * password is discarded). */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/error_type.h"

extern void ui_loading_init_state(void);
extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);
extern void display_error_when_main_menu_loaded(int16_t error_code);

void main_connect_private(void)
{
    unsigned short wide_password[16];
    ui_loading_init_state();
    ascii_to_wide(main_globals.connect_password, wide_password, 0x12u);
    display_error_when_main_menu_loaded(_error_invalid_address);
    main_globals.switch_to_structure_bsp_index = -1;
    main_globals.want_to_be_at_main_menu = 1;
    main_globals.save_map = 0;
    main_globals.connect_address[0] = 0;
    main_globals.connect_password[0] = 0;
    main_globals.want_to_connect = 0;
}
