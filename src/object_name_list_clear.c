#include <string.h>
#include "headers/blam_data_globals.h"


void * object_name_list_clear(void)
{
    return memset(object_name_list, -1, 0x800);
}
