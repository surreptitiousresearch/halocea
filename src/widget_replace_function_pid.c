/* widget_replace_function_pid @0x83804020 — widget text-replace callback returning the console's PID
 * placeholder string, formatted into the shared result_3 buffer on first use. widget is unused. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/blam_data_globals.h"


extern void usprintf(wchar_t *string, const wchar_t *format, ...);

uint16_t * widget_replace_function_pid(widget_instance *widget)
{
    if (!result_3[0])
        usprintf(result_3, L"%S", "xxxx dddd dddd dddd dddd");

    return result_3;
}
