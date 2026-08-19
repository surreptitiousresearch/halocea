/* tag_loaded @ 0x836F6F78 — resolve a (group_tag, name) pair to its loaded tag index by
 * linear-scanning the tag-instance table. Returns -1 if tags aren't loaded or no match. */

#include <stdint.h>
#include "headers/cache_file_tags_globals.h"

extern int stricmp(const char *a, const char *b);

int tag_loaded(uint32_t group_tag, const char *name)
{
    cache_file_tags_header *tags_header;
    int i;

    if ( !cache_file_globals.tags_loaded )
        return -1;
    tags_header = cache_file_globals.tags_header;
    if ( tags_header->tag_count <= 0 )
        return -1;

    for ( i = 0; i < tags_header->tag_count; i = (int16_t)(i + 1) ) /* cast models the extsh @ 836F6FF4 */
    {
        cache_file_tag_instance *instance = &global_tag_instances[i];
        if ( group_tag == (uint32_t)instance->group_tag && !stricmp(name, instance->name) ) /* cmplw: unsigned compare */
            return global_tag_instances[i].tag_index;
    }
    return -1;
}
