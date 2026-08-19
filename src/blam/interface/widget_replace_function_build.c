/* widget_replace_function_build @0x83803FB8 — widget text-replace callback returning the build version
 * string, formatted into the shared result_2 buffer on first use. widget is unused. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/blam_data_globals.h"


extern void usprintf(wchar_t *string, const wchar_t *format, ...);

uint16_t * widget_replace_function_build(widget_instance *widget)
{
    if (!result_2[0])
        usprintf(result_2, L"%S", "01.00.01.0563");

    return result_2;
}
