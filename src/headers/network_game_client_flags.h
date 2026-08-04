#ifndef NETWORK_GAME_CLIENT_FLAGS_H
#define NETWORK_GAME_CLIENT_FLAGS_H

/* network_game_client_flags — network_game_client flags.
 * DB-verified: types_enum_values _55CA507D51CD2BD3C7FC2753F48E8AA1 carries these named
 * values verbatim. Bit-index members use as (1u << _bit); *_MASK / *_MAXIMUM
 * / large members are composite masks or constants, used as-is. */
enum network_game_client_flags {
	_network_game_client_connected_to_server_bit = 0,
	_network_game_client_sent_join_request_to_server_bit = 1,
	_network_game_client_want_to_abort_connection_attempt = 2,
	NUMBER_OF_NETWORK_GAME_CLIENT_FLAGS = 3,
	NETWORK_GAME_CLIENT_CONNECTION_PROCESS_TIMEOUT_MILLISECONDS_DEFAULT = 15000,
	NETWORK_GAME_CLIENT_CONNECTION_PROCESS_RETRY_MESSAGE_DELAY_MILLISECONDS_DEFAULT = 3000,
	NETWORK_GAME_CLIENT_GAME_SEARCH_INTERVAL_MILLISECONDS = 2000,
	NETWORK_GAME_CLIENT_GAME_ADVERTISED_GAME_TIMEOUT = 6000,
	NETWORK_GAME_CLIENT_GAME_PING_INTERVAL_MILLISECONDS = 3000,
	NETWORK_GAME_CLIENT_INITIAL_PACKET_SEQUENCE_NUMBER = 1,
	NUMBER_OF_SAVED_SERVER_GAME_UPDATE_MESSAGES = 128,
};

#endif /* NETWORK_GAME_CLIENT_FLAGS_H */

// network_game_client_flags: DB-verified via types_enum_values _55CA507D51CD2BD3C7FC2753F48E8AA1
