/* player_update_client_log @0x8376DAA8 — debug logging for client player-update history. Selects the
 * remote or local player log-level mask by subject, and when the requested log_level bits are all enabled,
 * formats the message and (if the client log destination includes the file bit, 0x2) appends it to the log
 * file.
 *
 * DEVIATION: the decompiler reported "local variable allocation has failed" and produced garbled va_list
 * handling plus spurious __int64 locals; this is the reconstructed intent (a standard vsprintf-then-append
 * forwarder). */

#include <stdarg.h>
#include <stdio.h>
#include "headers/blam_data_globals.h"
#include "headers/client_log_subject.h"

/* Attestation fix: binary does `lwz r3, s_file_name_0` then fopen(r3) @0x8376DB40 — the global is a
 * char* POINTER to "ClientPlayerUpdateHistory.log", not a char array (the old `extern char []` decl
 * would have passed the global's own address). Storage: src/data/s_file_name_0.c @0x8417EEC8; the
 * canonical extern now comes from headers/blam_data_globals.h, so the inline one is gone. */
extern int vsprintf_0(char *buffer, const char *format, va_list args);

void player_update_client_log(client_log_subject subject, int log_level, const char *format, ...)
{
    int level = subject ? remote_player_log_level : local_player_log_level;
    if ( (level & log_level) == log_level )
    {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsprintf_0(buffer, format, args);
        va_end(args);

        if ( (client_log_destination & 2) == 2 )
        {
            FILE *file = fopen(s_file_name_0, "a");
            if ( file )
            {
                fprintf(file, buffer);
                fclose(file);
            }
        }
    }
}
