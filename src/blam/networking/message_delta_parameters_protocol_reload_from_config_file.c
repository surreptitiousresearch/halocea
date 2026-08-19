/* message_delta_parameters_protocol_reload_from_config_file @0x83811A58 — when protocol changeover is
 * enabled, read the entire "parameters.cfg" file into the message-delta parameter string. */

#include <stdio.h>
#include "headers/blam_data_globals.h"


void message_delta_parameters_protocol_reload_from_config_file(void)
{
    if ( g_protocol_changeover_enabled == 1 )
    {
        FILE *file = fopen("parameters.cfg", "rb");
        if ( file )
        {
            fseek(file, 0, SEEK_END);
            unsigned int size = ftell(file);
            fseek(file, 0, SEEK_SET);
            /* CAVEAT: no bound against the 2048-byte g_parameters_string buffer — a larger
             * parameters.cfg overflows it (and the [size]=0 write). Faithful to the binary's
             * fseek/ftell/fread sequence; dev-only path behind g_protocol_changeover_enabled. */
            fread(g_parameters_string, 1u, size, file);
            g_parameters_string[size] = 0;
            fclose(file);
        }
    }
}
