#include "headers/blam_data_globals.h"
/* sv_dedicated_initialize @0x83765F18 — resets the dedicated-server console-variable state: clears the
 * server name/password buffers (partially — only their first 16 and 9 bytes respectively, per disasm;
 * matches the exact stw/stb sequence rather than a full sizeof() clear) and resets max-player-count and
 * public-visibility to their defaults. */


void sv_dedicated_initialize(void)
{
    char *name = (char *)sv_name_value;
    char *password = (char *)sv_password_value;

    /* Faithful partial byte-clear of the wide buffers (matches the raw stw/stb sequence):
     * cast-wrapped element writes so no clean struct member exists to name. */
    ((int *)name)[0] = 0;
    ((int *)password)[0] = 0;
    ((int *)name)[1] = 0;
    ((int *)name)[2] = 0;
    ((int *)password)[1] = 0;
    ((int *)name)[3] = 0;
    password[0x8] = 0;

    sv_max_players_value = 32;
    sv_public_value = 1;
}
