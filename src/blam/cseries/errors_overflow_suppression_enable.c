/* errors_overflow_suppression_enable @0x83764B40 — toggle suppression of error-log overflow messages. */

#include <stdint.h>
#include "headers/error_global_data.h"
#include "headers/blam_data_globals.h"


void errors_overflow_suppression_enable(uint8_t overflow_suppression)
{
    error_globals.overflow_suppression = overflow_suppression;
}
