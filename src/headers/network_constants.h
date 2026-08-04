#ifndef NETWORK_CONSTANTS_H
#define NETWORK_CONSTANTS_H

/* Network protocol / game sizing constants.
 * DB: anonymous enum _42E5B8B0E44AC2D566C9B0242A6316EB (types_enum_values). */

enum
{
    PLAYER_ACTION_PACKET_DEFINITION                          = 1,
    PLAYER_ACTION_COLLECTION_PACKET_DEFINITION               = 1,
    MAXIMUM_NUMBER_OF_LOCAL_PLAYERS                          = 2,
    MAXIMUM_NUMBER_OF_SCORED_TEAMS                           = 2,
    NETWORK_PROTOCOL_VERSION                                 = 6,
    MINIMUM_ACCEPTED_NETWORK_PROTOCOL_VERSION                = 6,
    MAXIMUM_ACCEPTED_NETWORK_PROTOCOL_VERSION                = 6,
    MINIMUM_MESSAGE_SIZE_IN_BITS                             = 8,
    MAXIMUM_NETWORK_GAME_SERVER_PASSWORD_LENGTH             = 8,
    MAXIMUM_NETWORK_GAME_PLAYER_NAME_LENGTH                  = 12,
    MAXIMUM_NETWORK_MACHINE_COUNT                            = 16,
    MAXIMUM_NETWORK_GAME_MACHINE_NAME_LENGTH                 = 16,
    MAXIMUM_NETWORK_GAME_NAME_LENGTH                         = 16,
    NETWORK_GAME_MAXIMUM_PLAYER_COUNT                        = 32,
    NETWORK_GAME_MAXIMUM_PLAYERS_PER_TEAM                    = 32,
    MAXIMUM_NETWORK_MAP_NAME_LENGTH                          = 128,
    NETWORK_GAME_SERVER_MAXIMUM_CLIENT_STALL_DURATION        = 2000,
    NETWORK_GAME_SERVER_SCAN_DURATION                        = 3000,
    NETWORK_SERVER_PORT                                      = 5150,
    NETWORK_CLIENT_PORT                                      = 5151,
    NETWORK_GAME_SERVER_MAXIMUM_WAIT_TIME_FOR_LEVEL_LOADING  = 60000,
    MESSAGE_READ_BUFFER_SIZE                                 = 65536
};

#endif /* NETWORK_CONSTANTS_H */
