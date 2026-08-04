#include "headers/blam_data_globals.h"

/* return const char*: propagates const of global_data_packet_groups_error_string (literal-only) 2026-07-31 (C4090) */
const char * data_packet_groups_get_error(void)
{
    const char *result = global_data_packet_groups_error_string;
    global_data_packet_groups_error_string = 0;
    return result;
}
