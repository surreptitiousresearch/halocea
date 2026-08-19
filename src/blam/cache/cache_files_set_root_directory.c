/* cache_files_set_root_directory @ 0x836F6D10
   Copies the given path into the global cache root directory buffer,
   truncating to 255 characters and guaranteeing NUL termination. */

#include <string.h>
#include "headers/blam_data_globals.h"


void cache_files_set_root_directory(const char *root_directory)
{
    strncpy(cache_root_directory, root_directory, 0x100u);
    cache_root_directory[255] = 0;
}
