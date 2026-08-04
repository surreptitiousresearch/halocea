#include "headers/blam_data_globals.h"
/* sv_parameters_dump @0x83765F00 — when the message-delta protocol changeover is enabled, write the
 * current protocol parameters to the server config file.
 * DEVIATION: on the disabled path the decompiler returns r3 uninitialized; the function is effectively
 * void there. Reproduced as an undefined return to match the binary. */

extern int message_delta_parameters_protocol_dump_to_config_file(void);

int sv_parameters_dump(void)
{
    int result;
    if ( g_protocol_changeover_enabled )
        return message_delta_parameters_protocol_dump_to_config_file();
    return result;  /* r3 undefined on this path */
}
