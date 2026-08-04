#include <stdint.h>
#include "headers/tag_iterator.h"
#include "headers/game_globals_tag_reference.h"
extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);

extern char *tag_get_name(int16_t tag_index);
extern char *strstr(const char *haystack, const char *needle);
extern void cheat_objects(const game_globals_tag_reference *references, int16_t name_count);

void cheat_all_chars(void)
{
    short count = 0;
    tag_iterator iterator;
    game_globals_tag_reference references[66];
    tag_iterator_new(&iterator, 0x756E6974u /* 'unit' */);
    for (int tag_index = tag_iterator_next(&iterator); tag_index != -1; tag_index = tag_iterator_next(&iterator))
    {
        if ((unsigned short)count >= 0x40)
            break;
        const char *name = tag_get_name(tag_index);
        if (name && strstr(name, "character"))
        {
            short idx = count++;
            references[idx].reference.index = tag_index;
        }
    }
    cheat_objects(references, count);
}
