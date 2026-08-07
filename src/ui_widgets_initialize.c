/* ui_widgets_initialize @ 0x837306F0 — allocate the 128 KiB widget pool and
 * reset widget globals to their "no deferred error" defaults. */

#include <string.h>
#include "headers/widget_globals.h"

#include "headers/stack_memory_pool.h"
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void stack_memory_pool_reset(stack_memory_pool *pool);

void ui_widgets_initialize(void)
{
    unsigned char initialized;
    unsigned char *pool;

    initialized = 1;
    pool = dlMalloc(
        0x20000, "D:\\Projects\\code\\HCEX\\sources\\interface\\ui_widget.c", 0x7A);
    if ( pool )
    {
        widget_memory_pool->base_address = pool;
        widget_memory_pool->pool_size = 0x20000;
    }
    else
    {
        initialized = 0;
    }
    stack_memory_pool_reset(widget_memory_pool);

    memset(&widget_globals, 0, sizeof(widget_globals));
    widget_globals.initialized = initialized;
    widget_globals.main_menu_deferred_error_code = -1;
    widget_globals.deferred_dashboard_error.error_code = -1;
    widget_globals.deferred_error[0].error_code = -1;
    widget_globals.fade_to_black = -1.0f;
    widget_globals.deferred_cinematic_error[0].error_code = -1;
    widget_globals.deferred_error[1].error_code = -1;
    widget_globals.deferred_cinematic_error[1].error_code = -1;
}
