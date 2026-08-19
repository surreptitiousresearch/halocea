#include "headers/blam_data_globals.h"
/* sv_parameters_reload @0x83765EC8 — when protocol changeover is enabled, reload the message-delta
 * parameter protocol from the config file and install the new parameter set.
 *
 * DEVIATION: the decompiler threads r3 between the three calls as if chained; the disassembly shows
 * no argument setup between them — they are independent no-argument calls. */

extern void message_delta_parameters_protocol_reload_from_config_file(void);
extern void message_delta_processor_parameters_reload(void);
extern void message_delta_parameters_protocol_establish_new_parameter_set(void);

void sv_parameters_reload(void)
{
    if ( g_protocol_changeover_enabled )
    {
        message_delta_parameters_protocol_reload_from_config_file();
        message_delta_processor_parameters_reload();
        message_delta_parameters_protocol_establish_new_parameter_set();
    }
}
