/* tag_iterator_next @0x836F70B0 — advance a tag iterator to the next tag instance whose group (or either
 * parent group) matches the iterator's key group (or any, when the key is -1). Returns that tag's index, or
 * -1 when the iteration is exhausted. */

#include <stdint.h>
#include "headers/tag_iterator.h"
#include "headers/cache_file_tags_globals.h"
#include "headers/cache_file_tag_instance.h"

int tag_iterator_next(tag_iterator *iterator)
{
    if ( iterator->iterator.absolute_index >= cache_file_globals.tags_header->tag_count )
        return -1;

    cache_file_tag_instance *instance;
    for ( ;; )
    {
        int index = iterator->iterator.absolute_index;
        iterator->iterator.absolute_index = index + 1;
        instance = &global_tag_instances[index];

        uint32_t key = iterator->key_group_tag;
        if ( key == (uint32_t)-1                              /* raw -1 sentinel; cmplw unsigned compares */
             || key == (uint32_t)instance->group_tag
             || key == (uint32_t)instance->parent_group_tags[0]
             || key == (uint32_t)instance->parent_group_tags[1] )
            break;

        if ( iterator->iterator.absolute_index >= cache_file_globals.tags_header->tag_count )
            return -1;
    }
    return instance->tag_index;
}
