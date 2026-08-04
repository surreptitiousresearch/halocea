/* player_profile_color_picker_menu_dispose @0x83696290 — widget dispose callback for the profile
 * color-picker menu: frees the list-parameter item buffer (if any) and releases the shared preview list. */
#include <stdint.h>

#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void widget_free(void *ptr);
extern void preview_list_free(void);

uint8_t player_profile_color_picker_menu_dispose(widget_instance *widget, event_record *event,
        unsigned __int8 *widget_deleted)
{
    void *list_items = widget->parameters.list_parameters.list_items;
    if ( list_items )
    {
        widget_free(list_items);
        widget->parameters.list_parameters.list_items = nullptr;
    }
    preview_list_free();
    return 1;
}
