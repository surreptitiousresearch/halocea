/* message_delta_parameters_protocol_dump_to_config_file @0x83811A0C — when protocol changeover is
 * enabled, write the current message-delta parameter string out to "parameters.cfg". */

#include <stdio.h>
#include "headers/blam_data_globals.h"


void message_delta_parameters_protocol_dump_to_config_file(void)
{
    if ( g_protocol_changeover_enabled == 1 )
    {
        FILE *file = fopen("parameters.cfg", "wb");
        if ( file )
        {
            fprintf(file, g_parameters_string);
            fclose(file);
        }
    }
}
