/* hcex_set_explode_grunts @0x83683D38 — hcex bridge skull/feature toggle: for every 'unit' tag whose name
 * is exactly "characters\grunt\grunt", set (val != 0) or clear bit 1 (0x2) of the unit definition's flags
 * dword at +380 (the "unit has boarding seats"-adjacent flag repurposed as explode-on-death for grunts).
 *
 * DEVIATION (sibling-reuse): the compiled body inlines a plain strcmp; the CRT strcmp is called instead. */

#include <stdint.h>
#include "../headers/tag_iterator.h"
#include "../headers/unit_definition.h"
#include "../headers/unit_definition_flags.h"
#include "../headers/global_tag_instances.h"

extern "C" void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern "C" int tag_iterator_next(tag_iterator *iterator);
extern "C" char *tag_get_name(int16_t tag_index);
extern "C" int strcmp(const char *s1, const char *s2);

extern "C" void hcex_set_explode_grunts(int val)
{
    tag_iterator iterator;
    tag_iterator_new(&iterator, 0x756E6974u /* 'unit' */);

    for ( int tag_index = tag_iterator_next(&iterator); tag_index != -1;
          tag_index = tag_iterator_next(&iterator) )
    {
        char *name = tag_get_name(tag_index);
        if ( name && !strcmp(name, "characters\\grunt\\grunt") )
        {
            unit_definition *definition = *(unit_definition **)TAG_INSTANCE(tag_index);
            /* bit 1 = _unit_is_destroyed_after_dying (DB $62D503FB72151F1243757361612DED81);
             * the grunt-explode skull toggles this definition flag. */
            if ( val )
                definition->unit.flags |= (1u << _unit_definition_destroyed_after_dying_bit);
            else
                definition->unit.flags &= ~(1u << _unit_definition_destroyed_after_dying_bit);
        }
    }
}
