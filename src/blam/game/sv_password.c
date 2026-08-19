#include <wchar.h>
#include <stdint.h>
#include "headers/virtual_keyboard_validate_mode.h"
#include "headers/blam_data_globals.h"
/* sv_password @0x837669B8 — "sv_password" server console command. With no argument it echoes the
 * current password. With one argument it validates the password (at most 8 characters, ASCII, and
 * renderable by the Halo UI font) and stores it as the live server password; an empty string clears
 * it. Any other arg count is a usage error. */


#include "headers/network_game_server.h"
extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);
extern uint8_t string_is_ui_compatible(const char *str, const wchar_t *wstr, int validate_mode);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern network_game_server *global_network_game_server_get(void);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void sv_password(int count, char **strings)
{
    char success = 0;

    if (!count)
    {
        success = 1;
    }
    else if (count == 1)
    {
        const char *password = strings[0];
        const char *scan = password;
        while (*scan++)
            ;
        if ((unsigned int)(scan - password - 1) > 8)
        {
            ship_terminal_printf(nullptr, "Server passwords must be no more than %d characters.", 8);
        }
        else
        {
            wchar_t wide_password[32];
            int valid = ascii_to_wide(password, wide_password, 0x12u) == wide_password;
            wide_password[8] = 0;
            if (valid)
            {
                const char *length_scan = password;
                while (*length_scan++)
                    ;
                if (length_scan - password != 1)   /* non-empty: must also be UI-renderable */
                    valid = string_is_ui_compatible(password, wide_password, _virtual_keyboard_validate_none);
            }
            if (valid)
            {
                global_network_game_server_get();
                ustrncpy(sv_password_value, wide_password, 8u);
                sv_password_value[8] = 0;
                success = 1;
            }
            else
            {
                ship_terminal_printf(nullptr,
                    "Server passwords must only contain printable ASCII characters supported by the Halo UI.");
            }
        }
    }

    if (success)
        ship_terminal_printf(nullptr, "sv_password: %ls", sv_password_value);
    else
        ship_terminal_printf(nullptr, "Incorrect usage. Type help sv_password for more information.");
}
