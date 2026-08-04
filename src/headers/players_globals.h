#pragma once
/* players_global_data — global player-management state (players.c). Layout from the database. The byte at
 * 0x1F packs two nibbles: the low nibble is a recursive BSP-switch settle countdown, the high nibble is
 * the local player who triggered the pending switch. */

#include "players_global_data.h"  /* canonical home: players_global_data type + players_globals extern */
