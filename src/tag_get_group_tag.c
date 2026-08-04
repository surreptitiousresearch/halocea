/* tag_get_group_tag @0x836F7540 — return the four-character group tag of a loaded tag (e.g. 'actr' for an
 * actor definition), read from its cache_file_tag_instance. */

#include <stdint.h>
#include "headers/cache_file_tag_instance.h"
#include "headers/blam_data_globals.h"


uint32_t tag_get_group_tag(int16_t tag_index)
{
    return global_tag_instances[tag_index].group_tag;
}
