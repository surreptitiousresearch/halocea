/* select_players_to_display @0x8374BE28 — build the scoreboard's visible player list: fills a 19-slot
 * scratch array via populate_statistic_buffer (already sorted by whichever stat the caller asked for),
 * then — if more players exist than the caller's requested display count — makes sure every LOCAL
 * (split-screen) player is visible even if their rank falls below the cutoff, by evicting a non-local
 * player from the tail of the visible list to make room. Copies up to `requested_count` entries into
 * the caller's buffer and returns how many were copied.
 *
 * DEVIATION: the DB's own applied prototype names these params (player_index, out_statistic_buffer,
 * postgame_statistic, maximum_count), but the body's actual use is different: the 1st param is the
 * memcpy DESTINATION (the real output buffer), the 2nd is the stat selector forwarded to
 * populate_statistic_buffer, the 3rd is the caller's requested display count, and the 4th is never read
 * at all. Renamed here to match actual behavior, confirmed via this function's own decompiled body. */

#include <stdint.h>
#include "headers/statistic_buffer.h"
#include "headers/postgame_statistic.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"


/* postgame_statistic is an enum tag (not a typedef) — must write `enum` in C. */
extern int populate_statistic_buffer(statistic_buffer *statistic_buffer, enum postgame_statistic statistic, uint8_t inverse);
extern void terminal_printf(const real_argb_color *color, const char *format, ...);
extern void *memmove(void *dest, const void *src, unsigned int count);
extern void *memcpy(void *dest, const void *src, unsigned int count);

enum postgame_statistic select_players_to_display(statistic_buffer *out_players, enum postgame_statistic statistic,
        int requested_count, int unused_max_capacity)
{
    statistic_buffer all_players[19];
    int total_count = populate_statistic_buffer(all_players, statistic, 0);

    int debug = rasterizer_debug_options.pad3 == 69;
    if (debug)
        terminal_printf(global_real_argb_white, "player_count=%d, maxcount=%d", total_count, requested_count);

    if (requested_count < total_count)
    {
        /* Collect any local players that fell beyond the visible cutoff. */
        statistic_buffer local_overflow[4];
        int local_overflow_count = 0;
        for (int i = requested_count; i < total_count; i++)
        {
            int player_index = all_players[i].player_index;
            player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
            if (player && (uint16_t)player->local_player_index != 0xFFFF)
            {
                if (debug)
                    terminal_printf(global_real_argb_white, "found local player");
                local_overflow[local_overflow_count] = all_players[i];
                local_overflow_count++;
            }
        }

        /* For each overflowed local player, evict the last non-local player from the visible list and
         * shift the remaining visible entries down to make room at the end. */
        for (int i = 0; i < local_overflow_count; i++)
        {
            int evict_index = requested_count - 1;
            while (evict_index >= 0)
            {
                int player_index = all_players[evict_index].player_index;
                if ((uint16_t)DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index == 0xFFFF)
                    break; /* found a non-local player slot to evict */
                evict_index--;
            }
            if (evict_index >= 0)
            {
                memmove(&all_players[evict_index], &all_players[evict_index + 1],
                        sizeof(statistic_buffer) * (requested_count - evict_index - 1));
                all_players[requested_count - 1] = local_overflow[i];
            }
        }
    }

    int copy_count = requested_count <= total_count ? requested_count : total_count;
    memcpy(out_players, all_players, sizeof(statistic_buffer) * copy_count);
    return copy_count;
}
