/* filesystem_check_thread_is_active @0x83730F98 */
#include "headers/widget_globals.h"

int filesystem_check_thread_is_active(void)
{
    return widget_globals.initialization_thread != 0;
}
