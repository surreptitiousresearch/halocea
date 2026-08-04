/* tag_get_name @ 0x836F7520
   Returns the path/name string of a tag by index. */

#include <stdint.h>

#include "headers/cache_file_tag_instance.h"

char *tag_get_name(int16_t tag_index)
{
    return global_tag_instances[tag_index].name;
}
