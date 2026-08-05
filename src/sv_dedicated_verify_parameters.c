#include <stdint.h>
#include "headers/blam_data_globals.h"
/* sv_dedicated_verify_parameters @0x8376700C — validate/report dedicated-server init.txt settings at startup:
 * defaults sv_name to "Halo" (with a warning) if left blank, then logs the effective sv_name, sv_maxplayers,
 * and sv_public_value to the ship terminal. */


extern unsigned int ustrlen(const wchar_t *string);
extern void ustrcpy(wchar_t *dest, const wchar_t *src);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void sv_dedicated_verify_parameters(void)
{
    if ( ustrlen(sv_name_value) )
    {
        ship_terminal_printf(0, "sv_name: %ls", sv_name_value);
    }
    else
    {
        ustrcpy(sv_name_value, (const uint16_t *)L"Halo");
        ship_terminal_printf(0, "WARNING: sv_name unspecified in init.txt, defaulting to %ls", sv_name_value);
    }

    ship_terminal_printf(0, "sv_maxplayers: %d", sv_max_players_value);
    ship_terminal_printf(0, "sv_public_value: %d", sv_public_value);
}
