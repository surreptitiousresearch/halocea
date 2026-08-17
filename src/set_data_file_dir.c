/* set_data_file_dir @0x83809760 */
#include "headers/blam_data_globals.h"

void set_data_file_dir(const char *new_dir, int need_reorder)
{
    data_file_reorder_on_write = need_reorder;
    data_file_dir              = new_dir;
}
