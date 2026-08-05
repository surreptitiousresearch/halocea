/* game_statistics_record_kill @0x83755228 — record a kill (and its assists) into the per-player game
 * statistics. For a player victim it bumps the death/suicide counters and resets the victim's own kill
 * streak. It then compacts the victim object's recent-damage history (dropping records whose damager player
 * is no longer valid) and, when there is no legitimate enemy killer, searches that history for the enemy
 * damager who dealt the most recent significant damage to credit with the kill. The credited killer gets a
 * kill (and multikill streak) if an enemy, or a betrayal/friendly-kill if not; every other damager above
 * 40%% of the credited damage gets an assist. Finally game_engine_player_killed is notified.
 *
 * Player statistics are the typed player_datum.statistics (game_statistics) block; the damage history is
 * the unit's attackers[4] records viewed as damage_history_record. `victim_flag` is the v10/v15 path flag,
 * which is always 0 here (the whole body runs only for a player victim), so `2*victim_flag` resolves to 0 —
 * it is reproduced verbatim from the binary. The damager player index at record+12 is passed to
 * game_team_is_enemy exactly as the original does (its team args equal the player slot in free-for-all). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_statistics_globals.h"
#include "headers/damage_history_record.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


extern int player_index_from_unit_index(int unit_index);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern int game_time_get(void);
extern void *datum_try_and_get(const data_array *data, int index);
extern void *memcpy(void *dst, const void *src, unsigned int count);
extern void game_engine_player_killed(int killing_player_index, int killing_object_index, int dead_player_index, uint8_t friendly_fire);

#define PLAYER_DATUM(player_index) DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)

void game_statistics_record_kill(int object_index, int owner_player_index, int owner_object_index,
                                 int16_t owner_team_index)
{
    if (!game_statistics_globals.active)
        return;

    int victim_player_index = player_index_from_unit_index(object_index);
    int victim_team = -1;
    int victim_flag;
    int16_t killer_own_record_index = -1;

    if (victim_player_index == -1)
    {
        victim_flag = -1;
    }
    else
    {
        victim_flag = 0;
        player_datum *victim = PLAYER_DATUM(victim_player_index);
        victim_team = victim->team_index;
        if (owner_player_index == victim_player_index)
            ++victim->statistics.suicides;
        int16_t deaths = victim->statistics.deaths;
        victim->statistics.kills_in_a_row = 0;               /* reset on death */
        victim->statistics.last_kill_time = -1;
        victim->statistics.multiple_kills = 0;
        victim->statistics.deaths = deaths + 1;
    }

    if (victim_flag == -1)
        return;

    /* a kill counts toward assists/credit when there is no legitimate enemy killer */
    uint8_t no_legit_killer = (owner_player_index == -1
                                       || !game_team_is_enemy(victim_team, owner_team_index));

    int multikill_window = game_time_get() - 120;   /* ~4s: streak/multikill reset window */
    int assist_window = game_time_get() - 180;       /* ~6s: damage-history relevance window */

    unit_datum *victim_unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    /* the unit's attackers[4] records, viewed through the damage-history shape */
    damage_history_record *history = (damage_history_record *)victim_unit->unit.attackers;

    /* compact the history: keep only records whose damager player is still a live datum, pad with empties */
    damage_history_record compacted[4];
    for (int i = 0; i < 4; i++)
    {
        compacted[i].time = 0;
        compacted[i].damage = 0.0f;
        compacted[i].damager_object_index = -1;
        compacted[i].damager_player_index = -1;
    }
    damage_history_record *dst = compacted;
    for (int i = 0; i < 4; i++)
    {
        if (datum_try_and_get(player_data, history[i].damager_player_index))
            *dst++ = history[i];
    }
    memcpy(history, compacted, sizeof(compacted));

    /* find the enemy damager with the most recent significant damage (and the killer's own record index) */
    int best_record_index = -1;
    float best_damage = -3.4028235e38f;   /* -FLT_MAX */
    for (int record_index = 0; record_index < 4; record_index++)
    {
        int damager_player = history[record_index].damager_player_index;
        if (damager_player == owner_player_index)
            killer_own_record_index = record_index;
        if ((killer_own_record_index == record_index
             || (no_legit_killer && game_team_is_enemy(victim_team, damager_player)))
            && history[record_index].time > assist_window
            && history[record_index].damage > best_damage)
        {
            best_record_index = record_index;
            best_damage = history[record_index].damage;
        }
    }

    uint8_t betrayal = 0;
    if (best_record_index == -1)
        best_record_index = killer_own_record_index;

    int credited_player;
    float assist_threshold;
    if (best_record_index == -1)
    {
        credited_player = owner_player_index;
        assist_threshold = 0.0f;
    }
    else
    {
        credited_player = history[best_record_index].damager_player_index;
        assist_threshold = history[best_record_index].damage * 0.40000001f;
    }

    if (credited_player != -1)
    {
        player_datum *killer = PLAYER_DATUM(credited_player);
        if (game_team_is_enemy(victim_team, killer->team_index))
        {
            ++killer->statistics.kills[victim_flag];
            if (!victim_flag)
            {
                int16_t last_kill_time = killer->statistics.last_kill_time;
                ++killer->statistics.kills_in_a_row;
                if (last_kill_time < multikill_window)
                    killer->statistics.multiple_kills = 1;
                else
                    ++killer->statistics.multiple_kills;
                killer->statistics.last_kill_time = game_time_get();
            }
        }
        else
        {
            betrayal = 1;
            ++killer->statistics.friendly_fire_kills;
            if (credited_player != victim_player_index)
                ++killer->statistics.killed_teammate_since_last_death;
        }
    }

    /* credit assists to every other enemy damager above the threshold (or the killer's own record) */
    if (assist_threshold > 0.0f)
    {
        for (int i = 0; i < 4; i++)
        {
            if (i == killer_own_record_index || history[i].damage > assist_threshold)
            {
                int assister_player = history[i].damager_player_index;
                if (assister_player != -1 && assister_player != credited_player)
                {
                    player_datum *assister = PLAYER_DATUM(assister_player);
                    if (game_team_is_enemy(victim_team, assister->team_index))
                        ++assister->statistics.assists[victim_flag];
                }
            }
        }
    }

    if (!victim_flag)
        game_engine_player_killed(credited_player, owner_object_index, victim_player_index, betrayal);
}
