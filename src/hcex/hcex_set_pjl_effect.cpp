/* hcex_set_pjl_effect @0x83682700 — select the "projectile" effect tag whose name contains the given
 * substring, caching its tag index in hcex_pjl_effect (-1 when none / no name given). Iterates every 'effe'
 * (effect) tag and stops at the first whose full name contains `name`. */

#include <stdint.h>
#include "../headers/tag_iterator.h"

extern int hcex_pjl_effect;
extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);
extern char *tag_get_name(int16_t tag_index);
extern char       *strstr(const char *haystack, const char *needle);

extern "C" void hcex_set_pjl_effect(const char *name)
{
    hcex_pjl_effect = -1;
    if ( !name || !*name )
        return;

    tag_iterator iterator;
    tag_iterator_new(&iterator, 'effe');   /* 0x65666665 — effect tag group */

    for ( int tag_index = tag_iterator_next(&iterator);
          tag_index != -1;
          tag_index = tag_iterator_next(&iterator) )
    {
        if ( strstr(tag_get_name(tag_index), name) )
        {
            hcex_pjl_effect = tag_index;
            return;
        }
    }
}
