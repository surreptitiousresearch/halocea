#include "headers/blam_data_globals.h"
/* sv_parameters_dump @0x83765F00 — when the message-delta protocol changeover is enabled, write the
 * current protocol parameters to the server config file.
 * DEVIATION: the beqlr @0x83765F0C path has no r3 def anywhere (an int return would be undefined
 * there), the tail-b's value is unconsumed by the sole caller sv_parameters_dump_evaluate
 * @0x8372E2C4, and its hs_function_table entry declares hs_type_void — void. */

extern int message_delta_parameters_protocol_dump_to_config_file(void);

void sv_parameters_dump(void)
{
    if ( g_protocol_changeover_enabled )
        message_delta_parameters_protocol_dump_to_config_file();
}
