#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void main_goto_main_menu(void);

uint8_t pause_game_quit_to_main_menu(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    main_goto_main_menu();
    return 1;
}
