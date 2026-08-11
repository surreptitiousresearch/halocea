/* ui_widgets_dispose @ 0x837343C0 — close widgets, free the widget pool, zero globals */
#include <string.h>
#include "headers/widget_globals.h"
extern void ui_widgets_close_all(void);
extern void dlFree(void *ptr);
void ui_widgets_dispose(void)
{
    ui_widgets_close_all();
    if ( widget_memory_pool->base_address )
        dlFree(widget_memory_pool->base_address);
    widget_memory_pool->base_address = 0;
    widget_memory_pool->pool_size = 0;
    /* tail memset return residue; 0 consumers -> void */
    memset(&widget_globals, 0, sizeof(widget_globals));
}
