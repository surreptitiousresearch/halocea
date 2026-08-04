/* players_get_respawn_failure @0x836A8010 — the current respawn failure reason code (players_globals +0x1C). */

#include <stdint.h>
#include "headers/players_globals.h"

int16_t players_get_respawn_failure(void)
{
    return (unsigned short)players_globals->respawn_failure_reason;
}
