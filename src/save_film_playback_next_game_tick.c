/* save_film_playback_next_game_tick @ 0x836EB968 — read and decode the next film frame:
 * a header packet giving the body size, then the body packet carrying a server update.
 * Validates the game-state CRC, applies the recorded player 0 facing/zoom, hands the
 * update to the client, and advances the expected-update counter. Returns 1 on success. */
#include <stdint.h>
#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"
#include "headers/game_update.h"
#include "headers/real_vector3d.h"

typedef struct data_packet_group_definition data_packet_group_definition;

#include "headers/server_update_packet.h"
#include "headers/saved_film_frame_header_packet.h"
#include "headers/blam_data_globals.h"

#include "headers/real_euler_angles2d.h"
extern saved_film_frame_header_packet_t saved_film_frame_header_packet;

extern uint8_t saved_film_read(void *buffer, int16_t *size);
extern uint8_t data_packet_group_decode_packet(data_packet_group_definition *group_definition, void *decoded_packet, char *encoded_packet, int16_t *encoded_packet_size, int16_t *packet_type, int16_t *packet_version, int16_t expected_packet_class);
extern uint32_t game_state_calculate_crc(void);
extern void __trap(void);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern void player_control_set_facing(int16_t local_player_index, const real_vector3d *forward);
extern void player_control_set_zoom_level(int16_t local_player_index, int16_t zoom_level);
extern void update_client_handle_server_update(game_update *new_update, int update_number);
extern void *memcpy(void *, const void *, unsigned int);

uint8_t save_film_playback_next_game_tick(void)
{
    __int16 packet_type_body = _message_server_game_update_packet;
    __int16 packet_type_header = _saved_film_frame_header_packet;
    __int16 packet_version = SAVED_FILM_VERSION;
    __int16 body_size;
    __int16 header_size;
    __int16 decoded_body_size;
    unsigned char header_buf[8];
    unsigned char body_buf[400];
    server_update_packet decoded;
    real_vector3d facing;
    game_update update;
    unsigned __int16 action_count;
    __int16 applied_count;
    int k;

    header_size = saved_film_frame_header_packet.size;
    if ( !saved_film_read(header_buf, &header_size) )
        return 0;
    if ( !data_packet_group_decode_packet(&saved_film_group, &decoded_body_size, header_buf,
                                          &header_size, &packet_type_header, &packet_version,
                                          _saved_film_packet_class) )
        return 0;

    body_size = decoded_body_size;
    if ( decoded_body_size <= 0 || (unsigned __int16)decoded_body_size > 0x190
      || !saved_film_read(body_buf, &body_size)
      || !data_packet_group_decode_packet(&saved_film_group, &decoded, body_buf,
                                          &body_size, &packet_type_body, &packet_version,
                                          _saved_film_packet_class) )
        return 0;

    if ( decoded.game_state_crc != game_state_calculate_crc() )
    {
        __trap();   /* desync — film does not match this build's game state */
        return 0;
    }

    if ( decoded.update_number != (int)global_saved_film.next_expected_update )
        return 0;

    action_count = decoded.player_count;
    if ( action_count > 0x20 )
        return 0;

    update.number_of_actions = action_count;
    if ( action_count )
    {
        global_saved_film.player_count = action_count;
        memcpy(global_saved_film.action_updates, decoded.action_update, 32 * action_count);
        applied_count = global_saved_film.player_count;
    }
    else
    {
        applied_count = global_saved_film.player_count;
        if ( global_saved_film.player_count > 0 )
        {
            action_count = global_saved_film.player_count;
            update.number_of_actions = global_saved_film.player_count;
        }
    }

    if ( !action_count )
        return 0;

    memcpy(update.actions, global_saved_film.action_updates, 32 * applied_count);
    for ( k = 0; k < update.number_of_actions; ++k )
        update.completed_client_update_id[k] = -1;

    player_control_set_facing(0, vector3d_from_euler_angles2d(&facing, &update.actions[0].desired_facing));
    player_control_set_zoom_level(0, update.actions[0].desired_zoom_level);
    update_client_handle_server_update(&update, decoded.update_number);
    ++global_saved_film.next_expected_update;
    return 1;
}
