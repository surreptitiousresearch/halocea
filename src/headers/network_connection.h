#pragma once

/* Opaque forward declaration only: the retail build's built-in Blam networking stack
 * (network_game_server/network_game_client/network_connection) is compiled out — every consumer seen so
 * far is a constant-return stub whose network_connection* parameter/return is never dereferenced. Full
 * layout deferred until a function that actually reads its fields is decompiled. */
typedef struct network_connection network_connection;
