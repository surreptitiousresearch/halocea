#include "headers/error_global_data.h"

void errors_clear(void)
{
    error_globals.delayed = 0;
    error_globals.message_buffer_size = 0;
}
