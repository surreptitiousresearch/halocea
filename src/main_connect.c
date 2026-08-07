/* main_connect @0x83688D58 — set up a pending client connection. Called with (null, null) it clears the
 * stored connect address/password and the want-to-connect flag and reports success. Otherwise it stores the
 * password, recurses to clear the request, and raises display error 53 (the address path is compiled out in
 * this build, so a non-null call always fails).
 *
 * Verified against disassembly: strncpy dest is connect_password (main_globals+0x3BA); display_error code is
 * 0x35 (53). Faithful to the compiled (degenerate) control flow. */

#include <stdint.h>
#include <string.h>
#include "headers/main_globals.h"
#include "headers/error_type.h"

extern void display_error(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);

int main_connect(const char *ip_and_port, const char *password)
{
    int cleared = 0;

    if ( !ip_and_port && !password )
    {
        cleared = 1;
        main_globals.connect_address[0] = 0;
        main_globals.connect_password[0] = 0;
        main_globals.want_to_connect = 0;
    }
    else
    {
        strncpy(main_globals.connect_password, password, 8u);
        main_globals.connect_password[8] = 0;
    }

    if ( !cleared )
    {
        main_connect(0, 0);
        display_error(_error_invalid_address, -1, 1u, 0);
    }

    return cleared;
}
