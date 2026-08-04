#pragma once
/* connected_rejection_code — reason a client machine's connection attempt was rejected by
 * network_game_server_reject_connected_client_machine. */

typedef enum connected_rejection_code
{
    connected_rejection_code_unknown              = 0,
    connected_rejection_code_bad_join_token        = 1,
    connected_rejection_code_bad_password          = 2,
    connected_rejection_code_game_is_full          = 3,
    connected_rejection_code_invalid_cd_key        = 4,
    connected_rejection_code_invalid_cd_key_inuse  = 5,
    connected_rejection_code_blacklisted_cd_key    = 6,
    connected_rejection_code_kicked                = 7,
    NUMBER_OF_CONNECTED_REJECTION_CODES            = 8,
} connected_rejection_code;
