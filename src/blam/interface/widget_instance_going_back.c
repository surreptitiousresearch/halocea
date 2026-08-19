#include <stdint.h>
#include "headers/blam_data_globals.h"
/* widget_instance_going_back @0x837329B0 — reports whether a widget is currently being reloaded as part of
 * widget_instance_go_back_to_previous's back-stack pop (see that function; it flips this flag around the
 * reload call). */


uint8_t widget_instance_going_back(void)
{
    return widget_instance_going_back_to_previous;
}
