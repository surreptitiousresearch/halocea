/* ai_scripting_allegiance @0x83770B40 — script command: make two teams allied. Most pairings are unconditional
 * alliances, but when the player team is allied with the human or sentinel team the alliance is made
 * "suspicious": it carries a difficulty-scaled incident decay time (so friendly fire can break it over time),
 * a fixed incident threshold of 5, and — for the human team — requires communication. The suspicious flag is set
 * on whichever of the two teams is the special (human/sentinel) team.
 * Team literals bind to enum game_team (DB $5D2EF8FA442E585DBFF707A5F144D58C): player=1, human=2, sentinel=5. */

#include <stdint.h>
#include "headers/game_allegiance.h"
#include "headers/game_team.h"

extern int16_t game_difficulty_level_get(void);
extern void game_allegiance_create(int16_t team1_index, uint8_t team1_suspicious, int16_t team2_index, uint8_t team2_suspicious, int16_t incident_threshold, int16_t incident_decay_time, uint8_t allegiance_requires_communication);

void ai_scripting_allegiance(int16_t team1_index, int16_t team2_index)
{
    if ( team1_index == -1 || team2_index == -1 )
        return;

    int16_t special_team = -1;
    int16_t incident_decay_time = -1;
    int16_t incident_threshold = -1;
    unsigned char requires_communication = 0;
    char suspicious_pairing = 0;

    if ( team1_index == _game_team_player )
        special_team = team2_index;
    else if ( team2_index == _game_team_player )
        special_team = team1_index;

    if ( special_team == _game_team_human || special_team == _game_team_sentinel )
    {
        static const int16_t decay_times_by_difficulty[4] = { 300, 450, 1200, 2700 };
        suspicious_pairing = 1;
        incident_threshold = 5;
        incident_decay_time = decay_times_by_difficulty[game_difficulty_level_get()];
        requires_communication = special_team == _game_team_human;
    }

    unsigned char team2_suspicious = suspicious_pairing && team2_index == special_team;
    unsigned char team1_suspicious = suspicious_pairing && team1_index == special_team;

    game_allegiance_create(team1_index, team1_suspicious, team2_index, team2_suspicious,
                           incident_threshold, incident_decay_time, requires_communication);
}
