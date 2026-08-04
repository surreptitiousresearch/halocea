#pragma once
/* client_log_subject — which side of a client-visible player_update_client_log() message
 * concerns (DB enum, pdb-resolved). */

typedef enum client_log_subject
{
    _client_log_local_player = 0,
    _client_log_remote_player = 1,
} client_log_subject;
