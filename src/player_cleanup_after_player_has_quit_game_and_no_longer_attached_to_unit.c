/* player_cleanup_after_player_has_quit_game_and_no_longer_attached_to_unit @0x836A7DE0 — final cleanup
 * for a player who has left and whose unit is already gone: remove them from the client (and server, if
 * hosting) update lists, free the player datum (machine index at player record +100), drop their
 * network field-index mapping, and remove them from the game-engine score table. Player records are
 * stride 512. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/field_properties_definition.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

typedef struct _field_properties_definition _field_properties_definition;
extern void update_client_remove_player(int player_index);
extern int16_t game_connection(void);
extern void update_server_remove_player(int player_index);
extern void player_delete(int machine_index, int player_index);
extern int field_translated_index_unregister_local_index(const _field_properties_definition *field_properties_definition, int local_index);
extern void game_engine_player_score_remove_player(int player_index);

void player_cleanup_after_player_has_quit_game_and_no_longer_attached_to_unit(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    update_client_remove_player(player_index);
    if ( game_connection() == _game_connection_network_server )
        update_server_remove_player(player_index);
    player_delete(player->network_player_data.machine_index, player_index);
    field_translated_index_unregister_local_index(&field_properties_player_index_definition, player_index);
    game_engine_player_score_remove_player(player_index);
}
