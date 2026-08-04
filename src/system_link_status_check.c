#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/error_type.h"

extern uint8_t network_game_is_splitscreen_local(void);
extern void display_error_when_main_menu_loaded(int16_t error_code);
extern void main_goto_main_menu(void);

void system_link_status_check(widget_instance *widget)
{
    if ( !network_game_is_splitscreen_local() )
    {
        display_error_when_main_menu_loaded(_error_network_connection_lost); /* error_type domain (error_code param of display_error) */
        main_goto_main_menu();
    }
}
