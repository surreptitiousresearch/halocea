#include "headers/error_global_data.h"

char * error_get(void)
{
    return error_globals.message_buffer;
}
