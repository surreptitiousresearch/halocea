// aiTEAMS_SYS::PlayersTeamID @ 0x83172EA0
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/ds_new.h" // placement new (tracked new, size_t-correct)

// Lazily interns the reserved "PLAYERS" team id on first call (function-local static,
// guard byte _S1_241 + storage TEAM_PLAYERS_0 in the DB) and copies its id out.
dsSTRID *aiTEAMS_SYS::PlayersTeamID(dsSTRID *result)
{
    static unsigned char initialized = 0; // _S1_241
    static dsSTRID teamPlayers;           // TEAM_PLAYERS_0

    if ((initialized & 1) == 0) {
        initialized |= 1u;
        new (&teamPlayers) dsSTRID("PLAYERS", 0); // intern-and-look-up
    }
    result->id = teamPlayers.id;
    return result;
}
