#include <stdint.h>
#include "headers/cache_file_tag_instance.h"
#include "headers/blam_data_globals.h"


cache_file_tag_instance * cache_file_tag_instance_get(int16_t tag_index)
{
    return &global_tag_instances[tag_index];
}
