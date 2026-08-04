/* populate_statistic_buffer @0x8374BAB8 — fills `statistic_buffer` (up to 16 rows, one per connected
 * player) with the requested `statistic`, sorts it, and stamps each row's `place` (a "tied with the row
 * above" run — ties share the SAME place value, encoded with its top bit set as a "tied" flag; a
 * non-tied row's place is just its own rank index). Returns the number of rows filled.
 *
 * `statistic` selects what `score` holds: 0 = a combined view (adjusted individual score in `custom`,
 * plus raw `kills`/`deaths`/`assists` from the player's lifetime statistics) — `score` itself is left
 * UNTOUCHED in this case, a genuine quirk confirmed via disasm_range(0x8374BB70, 0x8374BC30): the
 * `case 0` block never stores to the `score` field before falling into the shared inverse-negate tail,
 * which reads-and-writes it regardless (this path sorts via `sort_statistic_buffer_ranking`, which
 * presumably keys off `custom`/`kills`/`deaths`/`assists` instead — not yet decompiled to confirm).
 * 1 = adjusted individual score only, 2 = kills, 3 = assists, anything else (including 4) = deaths.
 * `inverse` flips ascending/descending sort order, except for `statistic == 4` (deaths) where it is
 * inverted from the caller's request first — deaths naturally sort the opposite way from every other
 * stat.
 *
 * Clean decompile (no corruption); `game_statistics.kills[0]`/`assists[0]`/`deaths` field identities
 * confirmed via absolute-offset math against the already-resolved `player_datum.statistics` (0x90) and
 * `game_statistics` (established in an earlier session's `game_engine_rasterize_in_game_score` pass). */

#include <stdint.h>
#include <stdlib.h>

#include "headers/statistic_buffer.h"
#include "headers/postgame_statistic.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/game_engine.h"
#include "headers/blam_data_globals.h"

extern struct game_engine *game_engine;

extern int adjust_score_for_ranking(int player_index, int score);
extern int sort_statistic_buffer(const void *a, const void *b);
extern int sort_statistic_buffer_ranking(const void *a, const void *b);

int populate_statistic_buffer(statistic_buffer *statistic_buffer, enum postgame_statistic statistic, uint8_t inverse)
{
    unsigned __int8 apply_inverse = (statistic == _postgame_statistic_deaths) ? (inverse == 0) : inverse;

    int count = 0;
    data_iterator player_iterator;
    data_iterator_new(&player_iterator, player_data);
    while (data_iterator_next(&player_iterator))
    {
        if (count < 16)
        {
            statistic_buffer[count].player_index = player_iterator.index;
            ++count;
        }
    }

    for (int i = 0; i < count; ++i)
    {
        int player_index = statistic_buffer[i].player_index;
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

        if (statistic == _postgame_statistic_ranking)
        {
            statistic_buffer[i].custom = 0;
            if (game_engine->get_score)
                statistic_buffer[i].custom =
                        adjust_score_for_ranking(player_index, game_engine->get_score(player_index, _get_score_individual));
            statistic_buffer[i].kills = player->statistics.kills[0];
            statistic_buffer[i].assists = player->statistics.assists[0];
            statistic_buffer[i].deaths = player->statistics.deaths;
            /* score deliberately left untouched here — see DEVIATION note above */
        }
        else
        {
            int score;
            switch (statistic)
            {
            case _postgame_statistic_custom:
                score = 0;
                if (game_engine->get_score)
                    score = adjust_score_for_ranking(player_index,
                            game_engine->get_score(player_index, _get_score_individual));
                break;
            case _postgame_statistic_kills:
                score = player->statistics.kills[0];
                break;
            case _postgame_statistic_assists:
                score = player->statistics.assists[0];
                break;
            default:
                score = player->statistics.deaths;
                break;
            }
            statistic_buffer[i].score = score;
        }

        if (apply_inverse)
            statistic_buffer[i].score = -statistic_buffer[i].score;
    }

    qsort(statistic_buffer, count, sizeof(statistic_buffer[0]),
            statistic ? sort_statistic_buffer : sort_statistic_buffer_ranking);

    for (int i = 0; i < count; ++i)
    {
        if (i && statistic_buffer[i].custom == statistic_buffer[i - 1].custom
                && statistic_buffer[i].kills == statistic_buffer[i - 1].kills
                && statistic_buffer[i].deaths == statistic_buffer[i - 1].deaths
                && statistic_buffer[i].assists == statistic_buffer[i - 1].assists)
        {
            /* tied with the row above: propagate a shared "tied" place value to BOTH rows, taking the
             * row above's existing place (rank or an earlier tie chain) as the shared base. */
            int tied_place = statistic_buffer[i - 1].place | 0x80000000;
            statistic_buffer[i - 1].place = tied_place;
            statistic_buffer[i].place = tied_place;
        }
        else
        {
            statistic_buffer[i].place = i;
        }
    }

    return count;
}
