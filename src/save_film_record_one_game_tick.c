/* save_film_record_one_game_tick @0x836EB50C — while recording a saved film, encode and write one game
 * tick: the update number, RNG seed, and game-state CRC, plus the per-player actions when they differ from
 * the previously recorded tick (otherwise a player count of 0 is stored to delta-compress unchanged input).
 * The tick is encoded as a packet, prefixed with an encoded frame header carrying its size, and both are
 * appended to the film. Returns 1 on success (or when not recording / out of sequence) and 0 on failure.
 *
 * DEVIATION: the decompiler renders the action-buffer change test as a byte-sliding HIBYTE compare loop;
 * on this big-endian target that is a plain memcmp over 32*player_count bytes, written as such here. */

#include <stdint.h>
#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"
#include "headers/saved_film_game_tick.h"
#include "headers/player_action.h"
#include "headers/data_packet_group_definition.h"
#include "headers/blam_data_globals.h"


#include "headers/data_packet_group_definition.h"
extern uint32_t game_state_calculate_crc(void);
extern uint8_t data_packet_group_encode_packet(data_packet_group_definition *group_definition, void *decoded_packet, unsigned char *encoded_packet, int16_t *encoded_packet_size, int16_t packet_type, int16_t packet_version);
extern uint8_t saved_film_write(void *buffer, int16_t size);
extern void *memcpy(void *dst, const void *src, unsigned int size);
extern int  memcmp(const void *a, const void *b, unsigned int size);

uint8_t save_film_record_one_game_tick(unsigned int update_number, unsigned int random_seed, int16_t player_count, player_action *action_updates)
{
    if (global_saved_film.mode != _saved_film_recording_mode
        || global_saved_film.next_expected_update != update_number)
        return 1;

    saved_film_game_tick tick;
    tick.update_number = update_number;
    tick.random_seed = random_seed;
    tick.crc = game_state_calculate_crc();

    __int16 header_size = 8;
    __int16 body_size = 400;

    int actions_changed;
    if (player_count != global_saved_film.player_count)
        actions_changed = 1;
    else
        actions_changed = (player_count != 0)
                       && memcmp(action_updates, global_saved_film.action_updates, 32 * player_count) != 0;

    if (actions_changed)
    {
        global_saved_film.player_count = player_count;
        tick.player_count = player_count;
        memcpy(tick.action_updates, action_updates, 32 * player_count);
        memcpy(global_saved_film.action_updates, action_updates, 32 * player_count);
    }
    else
    {
        tick.player_count = 0;
    }

    unsigned char encoded_body[448];
    if (data_packet_group_encode_packet(&saved_film_group, &tick, encoded_body, &body_size,
                                        _message_server_game_update_packet, SAVED_FILM_VERSION))
    {
        __int16 frame_size = body_size;
        unsigned char encoded_header[8];
        if (body_size > 0
            && data_packet_group_encode_packet(&saved_film_group, &frame_size, encoded_header, &header_size,
                                               _saved_film_frame_header_packet, SAVED_FILM_VERSION)
            && saved_film_write(encoded_header, header_size)
            && saved_film_write(encoded_body, body_size))
        {
            ++global_saved_film.next_expected_update;
            return 1;
        }
    }
    return 0;
}
