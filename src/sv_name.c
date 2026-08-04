#include <wchar.h>
#include <stdint.h>
#include "headers/blam_data_globals.h"
/* sv_name @0x83766870 — "sv_name" server console command. With no argument it just echoes the current
 * server name. With one argument it validates the name (1–15 characters, ASCII, and renderable by the
 * Halo UI font) and, if valid, stores it as the live server name. Any other arg count is a usage error. */


#include "headers/network_game_server.h"
extern unsigned int strlen(const char *s);
extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);
extern uint8_t string_is_ui_compatible(const char *str, const wchar_t *wstr, int validate_mode);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern network_game_server *global_network_game_server_get(void);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void sv_name(int count, char **strings)
{
    char success = 0;

    if (!count)
    {
        success = 1;
    }
    else if (count == 1)
    {
        const char *name = strings[0];
        unsigned int length = strlen(name);
        if (length < 1 || length > 15)
        {
            ship_terminal_printf(nullptr, "Server names must be between 1 and %d characters.", 15);
        }
        else
        {
            wchar_t wide_name[32];
            int valid = ascii_to_wide(name, wide_name, 0x20u) == wide_name;
            wide_name[15] = 0;
            if (valid)
                valid = string_is_ui_compatible(name, wide_name, 3);
            if (valid)
            {
                global_network_game_server_get();
                ustrncpy(sv_name_value, wide_name, 0xFu);
                sv_name_value[15] = 0;
                success = 1;
            }
            else
            {
                ship_terminal_printf(nullptr,
                    "Server names must only contain printable ASCII characters supported by the Halo UI.");
            }
        }
    }

    if (success)
        ship_terminal_printf(nullptr, "sv_name: %ls", sv_name_value);
    else
        ship_terminal_printf(nullptr, "Incorrect usage. Type help sv_name for more information.");
}
