#include "headers/error_global_data.h"
#include "headers/developer_mode.h"
#include "headers/blam_data_globals.h"

extern void stack_walk_initialize(void);


void errors_initialize(void)
{
    /* slice candidate _developer_mode_conprint_enabled (=4) is value-wrong for 127;
     * DB enum $A2F90FFD... has _developer_mode_full = 127 exactly. */
    error_globals.developer_mode       = _developer_mode_full;
    error_globals.output_to_debug_file = 1;
    error_globals.overflow_suppression = 1;
    error_globals.suppress_all         = 0;
    error_globals.delayed              = 0;
    error_globals.message_buffer_size  = 0;
    stack_walk_initialize();
}
