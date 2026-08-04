#include <stdint.h>
/* game_statistics_record_damage @0x83755220 — hook for recording damage in the game-statistics system.
 * Stubbed out in this build: the body is empty (single blr). The parameters are retained for callers. */

void game_statistics_record_damage(int object_index, float damage, int owner_player_index, int owner_object_index, int16_t owner_team_index)
{
    (void)object_index;
    (void)damage;
    (void)owner_player_index;
    (void)owner_object_index;
    (void)owner_team_index;
}
