/* system_get_scratch_disk_root @ 0x83762E18
   Accessor for the scratch-disk root path string. */

#include "headers/scratch_disk_root.h"

char *system_get_scratch_disk_root(void)
{
    return scratch_disk_root;
}
