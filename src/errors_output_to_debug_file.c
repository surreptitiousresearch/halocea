#include <stdint.h>
#include "headers/error_global_data.h"

void errors_output_to_debug_file(uint8_t output_to_debug_file)
{
    error_globals.output_to_debug_file = output_to_debug_file;
}
