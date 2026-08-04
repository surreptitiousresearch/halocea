/* banlist_print_to_console @0x83766060 — print the ban list to the console, four entries per line, each
 * formatted "[<index> <12-wide name>]". Entries are 46 bytes; the name is at offset 0. Each line is built
 * by appending the per-entry strings (inline strcat) and printed in green. */

#include "headers/dynamic_array.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"

extern int sprintf_0(char *string, const char *format, ...);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void banlist_print_to_console(void)
{
    int index = 0;
    while ( index < banned_players.count )
    {
        int count_on_line = 0;
        char line[320];
        line[0] = 0;
        int offset = 46 * index;
        while ( index < banned_players.count )
        {
            char entry[64];
            sprintf_0(entry, "[%-3d %*s]", index, 12, (const char *)banned_players.elements + offset);

            char *dst = line;
            while ( *dst )
                ++dst;
            char *src = entry;
            char c;
            do
            {
                c = *src++;
                *dst++ = c;
            }
            while ( c );

            ++count_on_line;
            ++index;
            offset += 46;
            if ( count_on_line >= 4 )
                break;
        }
        ship_terminal_printf(global_real_argb_green, line);
    }
}
