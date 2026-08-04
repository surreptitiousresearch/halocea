#include "headers/blam_data_globals.h"
extern void message_delta_parameters_protocol_reload_from_config_file(void);

/* DEVIATION: DB return_type is unknown; disasm proves void — the false path is a bare
 * bnelr with no r3 write, and the true path tail-calls the void reload helper (0/1 callers
 * consume r3). Prior int-return with `return 0` invented a value the binary never produces. */
void message_delta_parameters_protocol_initialize(void)
{
    if (g_protocol_changeover_enabled == 1)
        message_delta_parameters_protocol_reload_from_config_file();
}
