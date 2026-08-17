/* object_name_list_clear @0x836EE068 */
#include <string.h>
#include "headers/blam_data_globals.h"


void * object_name_list_clear(void)
{
    return memset(object_name_list, -1, 0x800);
}
