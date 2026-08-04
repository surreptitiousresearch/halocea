/* hcex_dump_variants @0x836C8D10 — HCEX debug bridge: write the tag name of every loaded game-variant tag
 * ('actv') to "variants.txt", one per line. Opens the file, iterates the tag table for the variant group,
 * writes each tag's name, and closes the file. Does nothing if the file cannot be opened. */

#include <stdint.h>
#include <stdio.h>
#include "../headers/tag_iterator.h"

extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);
extern char *tag_get_name(int16_t tag_index);

extern "C" void hcex_dump_variants(void)
{
    FILE *file = fopen("variants.txt", "w");
    if ( !file )
        return;

    tag_iterator iterator;
    tag_iterator_new(&iterator, 0x61637476u /* 'actv' game variant */);
    for ( int tag_index = tag_iterator_next(&iterator); tag_index != -1;
          tag_index = tag_iterator_next(&iterator) )
    {
        fprintf(file, "%s\n", tag_get_name(tag_index));
    }
    fclose(file);
}
