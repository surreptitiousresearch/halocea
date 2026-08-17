/* reset_error_state @0x83764CF0 */
#include "headers/error_global_data.h"

void reset_error_state(void)
{
    error_globals.delayed = 0;
    error_globals.message_buffer_size = 0;
}
