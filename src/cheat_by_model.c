/* cheat_by_model @0x8369A138 — spawn one of each object tag whose model name matches mdl_name. Enumerates
 * every 'obje' tag, comparing each tag's name against the requested model via hcex_cmp_mdl_names, collecting
 * up to 64 matches into a tag-reference list, then hands them to cheat_objects to spawn in front of the
 * acting player.
 *
 * DEVIATION: the decompiler sizes the stack reference buffer at 66 elements (a frame-layout artifact); the
 * loop hard-caps collection at 64 (0x40), so the buffer is declared at 64 to match the enforced capacity. */

#include <stdint.h>
#include "headers/tag_iterator.h"
#include "headers/game_globals_tag_reference.h"

extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);
extern char *tag_get_name(int tag_index);
extern int hcex_cmp_mdl_names(const char *n1, const char *n2);
extern void cheat_objects(const game_globals_tag_reference *references, int16_t name_count);

void cheat_by_model(const char *mdl_name)
{
    int16_t match_count = 0;
    tag_iterator iterator;
    game_globals_tag_reference references[64];
    int tag_index;

    tag_iterator_new(&iterator, 0x6F626A65u);   /* 'obje' — object tag group */
    for ( tag_index = tag_iterator_next(&iterator); tag_index != -1; tag_index = tag_iterator_next(&iterator) )
    {
        if ( (uint16_t)match_count >= 0x40u )
            break;
        if ( hcex_cmp_mdl_names(tag_get_name(tag_index), mdl_name) )
            references[match_count++].reference.index = tag_index;
    }
    cheat_objects(references, match_count);
}
