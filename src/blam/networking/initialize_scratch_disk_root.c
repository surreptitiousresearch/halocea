/* initialize_scratch_disk_root @0x83763110 */
#include <string.h>
#include "headers/blam_data_globals.h"


char * initialize_scratch_disk_root(void)
{
    return strncpy(scratch_disk_root, "d:", 0x104u);
}
