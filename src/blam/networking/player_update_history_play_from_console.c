/* player_update_history_play_from_console @0x837ABC70 — debug console helper that replays the client
 * update record immediately following a given completed update id, applied to the first active
 * player's unit. Finds the record whose id == last_completed_update_id, advances to the next record,
 * and replays its starting position. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/data_iterator.h"
#include "headers/real_point3d.h"
#include "headers/player_update.h"
#include "headers/blam_data_globals.h"

typedef struct player_update_history player_update_history;
typedef struct local_player_vehicle_update_network_data local_player_vehicle_update_network_data;

#include "headers/network_game_client.h"
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern network_game_client *global_network_game_client_get(void);
extern void player_update_history_play(player_update_history *history, int unit_index, int last_completed_update_id, real_point3d starting_position, local_player_vehicle_update_network_data *vehicle_starting_info, uint8_t destructive, uint8_t show_results);

void player_update_history_play_from_console(int last_completed_update_id, uint8_t destructive)
{
    int unit_index = -1;
    data_iterator iterator;
    player_datum *player;

    /* Use the first active player's controlled unit. */
    data_iterator_new(&iterator, player_data);
    player = data_iterator_next(&iterator);
    if (player)
    {
        while ((uint16_t)player->local_player_index == 0xFFFF)  /* sentinel on signed __int16 -- skip empty slots */
        {
            player = data_iterator_next(&iterator);
            if (!player)
                goto have_unit;
        }
        unit_index = player->unit_index;
    }
have_unit:

    if (global_network_game_client_get())
    {
        /* DEVIATION: the update-record list head is read from the fixed low address 0x4 (confirmed by
         * the data xref + decompiler MEMORY[4]); the redundant second client_get is a null-check
         * artifact. Records are player_update nodes: id @0x00, biped_initial_state @0x2C
         * (starting position @0x30), next @0x414. */
        player_update *record = *(player_update **)4;
        int matched = 0;

        if (record)
        {
            while (1)
            {
                int is_match = (record->id == last_completed_update_id);
                record = record->next;  /* advance before testing the match */
                if (is_match)
                {
                    matched = 1;
                    break;
                }
                if (!record)
                    break;
            }
        }

        /* record now points at the entry following the matched id (or null). */
        if (matched && record)
        {
            player_update_history_play(
                0,
                unit_index,
                last_completed_update_id,
                record->biped_initial_state.position,  /* +0x30 = biped_initial_state.position (0x2C + 0x04) */
                0,
                destructive,
                0);
        }
    }
}
