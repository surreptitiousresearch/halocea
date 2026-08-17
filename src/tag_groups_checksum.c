/* tag_groups_checksum @0x836F6F50 */
#include "headers/cache_file_tags_globals.h"

unsigned int tag_groups_checksum(void)
{
    return cache_file_globals.tags_header->tags_checksum;
}
