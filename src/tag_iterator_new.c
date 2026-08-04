/* tag_iterator_new @0x836F70A0 — initialize a tag iterator to walk tags of the given group, from index 0. */
#include <stdint.h>
#include "headers/tag_iterator.h"
void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag)
{
    iterator->key_group_tag = key_group_tag;
    iterator->iterator.absolute_index = 0;
}
