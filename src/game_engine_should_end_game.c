/* game_engine_should_end_game @0x83750680 — server-only test for whether the current multiplayer game
 * should end. Ends immediately if only one team remains alive, or once the variant time limit is
 * reached. While the clock runs it broadcasts a countdown announcement to all players as the remaining
 * time crosses certain boundaries (every 1s under 5s left, the 30s mark, and every 60s/300s tick at 30
 * Hz further out). Returns 1 to end the game, 0 to keep playing. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/game_variant.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/game_connection.h"
#include "headers/game_engine_message.h"

struct game_engine;

extern struct game_engine *game_engine;

extern int16_t game_connection(void);
extern uint8_t multiple_teams_alive_for_should_end_game_test(void);
extern int game_time_get(void);
extern void multiplayer_message(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

uint8_t game_engine_should_end_game(void)
{
    if (!game_engine || game_connection() != _game_connection_network_server)
        return 0;
    if (!multiple_teams_alive_for_should_end_game_test())
        return 1;
    if (global_variant.universal_variant.time_limit <= 0)
        return 0;

    int remaining = global_variant.universal_variant.time_limit - game_time_get();
    if (remaining < 0)
        return 1;
    if (remaining == -1)
        return 0;
    if (!remaining)
        return 1;

    char announce = 0;
    if (remaining > 150)
    {
        if (remaining == 900)
        {
            announce = 1;
        }
        else
        {
            int boundary = (remaining >= 9000) ? 9000 * (remaining / 9000) : 1800 * (remaining / 1800);
            if (remaining == boundary)
                announce = 1;
        }
    }
    else
    {
        int boundary = 30 * (remaining / 30);
        if (remaining == boundary)
            announce = 1;
    }

    if (announce)
    {
        data_iterator iterator;
        data_iterator_new(&iterator, player_data);
        while (data_iterator_next(&iterator))
            multiplayer_message(iterator.index, iterator.index, game_engine_message_time_left, remaining, 1u);
    }
    return 0;
}
