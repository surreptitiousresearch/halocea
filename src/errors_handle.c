/* errors_handle @0x83764D50 — consume the pending "delayed" error flag and reset the error message buffer,
 * returning the flag value that was latched. Called once per frame. */

#include <stdint.h>
#include "headers/error_global_data.h"

uint8_t errors_handle(void)
{
    uint8_t result = error_globals.delayed;
    error_globals.delayed = 0;
    error_globals.message_buffer_size = 0;
    return result;
}
