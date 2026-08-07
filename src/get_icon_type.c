#include <stdint.h>
#include <wchar.h>
#include "headers/blam_data_globals.h"
/* get_icon_type @0x837317F0 — match a "%..."-escaped icon name at the start of a string against the
 * 40-entry `icon_names` table, returning its index or -1 if none matches. */


/* wcsnicmp declared by <wchar.h> */

int16_t get_icon_type(const wchar_t *string)
{
    int index;
    for (index = 0; index < 40; ++index)
    {
        if (!wcsnicmp(string, icon_names[index], wcslen(icon_names[index])))
            break;
    }

    return (int16_t)((index != 40) ? index : -1); /* callers extsh r3: declared width is 16-bit */
}
