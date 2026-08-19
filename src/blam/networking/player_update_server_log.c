/* player_update_server_log @0x83756D38 — debug logging: format a message and, when server player-update
 * history logging is enabled, append it to the log file.
 *
 * DEVIATION: the decompiler reported "local variable allocation has failed" and produced garbled
 * va_list handling plus spurious __int64 locals; this is the reconstructed intent (a standard
 * vsprintf-then-append-to-file forwarder). */

#include <stdarg.h>
#include <stdio.h>
#include "headers/blam_data_globals.h"


void player_update_server_log(const char *format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    if ( log_server_player_update_history == 1 )
    {
        FILE *file = fopen(s_file_name, "a");
        if ( file )
        {
            fprintf(file, buffer);
            fclose(file);
        }
    }
}
