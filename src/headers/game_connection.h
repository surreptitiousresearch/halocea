#pragma once

/* game_connection — the machine's role in the current game session (return value of game_connection()).
 * Source: reference enum _BA066E0BA8E585BE340A944102BAC737 (the IDA DB carries only the anonymized,
 * member-less enum shell; the named constants survive in headers_ref keyed by that hash). Tag-only enum
 * (no typedef) so it does not collide with the game_connection() accessor of the same name. */
enum game_connection
{
    _game_connection_local          = 0,
    _game_connection_network_client = 1,
    _game_connection_network_server = 2,
    _game_connection_film_playback  = 3,
    NUMBER_OF_GAME_CONNECTIONS      = 4,
};

// game_connection: DB-verified via types_enum_values _BA066E0BA8E585BE340A944102BAC737 (anonymous compiled enum, names sans leading _)
