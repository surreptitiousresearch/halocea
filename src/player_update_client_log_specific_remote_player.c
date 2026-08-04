/* player_update_client_log_specific_remote_player @0x8376E038 — debug logging: if the remote player's
 * name matches the currently tracked player name, format the message and emit it to the client log
 * under the remote-player subject.
 *
 * DEVIATION: the decompiler reported "local variable allocation has failed" and produced garbled
 * va_list handling and spurious __int64 locals; this is the reconstructed intent (a standard
 * vsprintf-then-log forwarder). The client-log subject constant _client_log_remote_player is 1
 * (li r3, 1 in the binary). */

#include <stdarg.h>
#include <stdio.h>
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/client_log_subject.h"

extern int ustrcmp(const wchar_t *string1, const wchar_t *string2);
extern void player_update_client_log(client_log_subject subject, int log_level, const char *format, ...);

void player_update_client_log_specific_remote_player(player_datum *remote_player, int log_level, const char *format, ...)
{
    if ( ustrcmp(remote_player->name, g_tracked_remote_player_position_updates) == 0 )
    {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        player_update_client_log(_client_log_remote_player, log_level, buffer);
    }
}
