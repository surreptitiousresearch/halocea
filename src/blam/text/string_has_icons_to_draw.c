#include <stdint.h>
#include <wchar.h>
#include "headers/blam_data_globals.h"
/* string_has_icons_to_draw @0x83731B90 — scan a wide string for any "%<icon-name>" escape recognized by
 * icon_names[40] (see get_icon_type.c). Returns true as soon as one matches. */


/* wcslen declared by <wchar.h> */
/* wcsnicmp declared by <wchar.h> */

uint8_t string_has_icons_to_draw(const wchar_t *string)
{
    while ( string )
    {
        wchar_t *percent = wcschr(string, '%');
        if ( !percent )
            break;

        const wchar_t *candidate = percent + 1;
        int index;
        for ( index = 0; index < 40; ++index )
        {
            if ( !wcsnicmp(candidate, icon_names[index], wcslen(icon_names[index])) )
                break;
        }

        if ( index != 40 )
            return 1;

        string = candidate;
    }

    return 0;
}
