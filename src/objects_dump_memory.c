/* objects_dump_memory @0x836EE678 — debug: append a report of object memory usage to "d:\object_memory.txt".
 * Objects are tallied two ways — by object type (12 fixed rows) and by tag definition (up to 1024 rows, with
 * an overflow warning) — accumulating per-group counts (active/garbage/dead/outside/at-rest) and sizes, then
 * each table is sorted by size and written.
 *
 * DEVIATION: the type-dump index expression 8*type + 8*ROL(type,1) reduces to 24*type (the dump_datum stride)
 * and is written as such; the per-row name pointer (lost by the decompiler) is reconstructed from
 * object_type_get_name / tag_get_name. */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/object_iterator.h"
#include "headers/object_header_datum.h"
#include "headers/dump_datum.h"
#include "headers/objects_information.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
extern void qsort(void *base, unsigned int count, unsigned int size, int (__fastcall *compare)(const void *, const void *));

#define MAXIMUM_DUMPS 1024

extern void object_add_to_dump(int object_index, dump_datum *dump);
extern void object_dump_write(struct _iobuf *stream, dump_datum *dump);
extern void objects_information_get(objects_information *information);
extern const char * object_type_get_name(int16_t object_type);
extern char *tag_get_name(int16_t tag_index);
extern int sort_dumps(const dump_datum *dump1, const dump_datum *dump2);

void objects_dump_memory(void)
{
    int definition_count = 0;
    __int16 overflow_count = 0;
    dump_datum type_dumps[12];
    dump_datum definition_dumps[MAXIMUM_DUMPS];

    memset(definition_dumps, 0, sizeof(definition_dumps));
    memset(type_dumps, 0, sizeof(type_dumps));
    for ( int type = 0; type < 12; type = (__int16)(type + 1) )
    {
        type_dumps[type].object_type = type;
        type_dumps[type].definition_index = -1;
    }

    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_all, 0);
    for ( _DWORD *object = object_iterator_next(&iterator); object; object = object_iterator_next(&iterator) )
    {
        /* Locate (or create) this object's definition row. */
        int definition_row = -1;
        for ( int row = 0; row < definition_count; row = (__int16)(row + 1) )
        {
            if ( definition_dumps[row].definition_index == *object )
            {
                definition_row = row;
                break;
            }
        }
        if ( definition_row == -1 )
        {
            if ( definition_count >= MAXIMUM_DUMPS )
            {
                ++overflow_count;
            }
            else
            {
                definition_row = definition_count;
                definition_dumps[definition_count].object_type = -1;
                definition_dumps[definition_count].definition_index = *object;
                ++definition_count;
            }
        }

        object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, iterator.index);
        if ( definition_row != -1 )
            object_add_to_dump(iterator.index, &definition_dumps[definition_row]);
        object_add_to_dump(iterator.index, &type_dumps[object_header->type]);
    }

    qsort(definition_dumps, definition_count, sizeof(dump_datum), sort_dumps);
    qsort(type_dumps, 12, sizeof(dump_datum), sort_dumps);

    FILE *file = fopen("d:\\object_memory.txt", "a+b");
    if ( !file )
        return;

    objects_information information;
    objects_information_get(&information);
    fprintf(file, "#%d objects (#%d active) using %3.2f%% of available memory\n\n",
            information.object_count, information.active_object_count, information.used_memory * 100.0f);

    fprintf(file, "OBJECTS BY TYPE\n");
    fprintf(file, "number (active) [garbage/   dead/outside/at-rest] maxsize totsize\n");
    for ( int i = 0; i < 12; i = (__int16)(i + 1) )
        object_dump_write(file, &type_dumps[i]);
    fprintf(file, "\n");

    fprintf(file, "OBJECTS BY DEFINITION\n");
    fprintf(file, "number (active) [garbage/   dead/outside/at-rest] maxsize totsize\n");
    for ( int i = 0; i < definition_count; i = (__int16)(i + 1) )
    {
        dump_datum *dump = &definition_dumps[i];
        const char *name = dump->definition_index == -1
                         ? ((unsigned __int16)dump->object_type != 0xFFFF ? object_type_get_name(dump->object_type) : "")
                         : tag_get_name(dump->definition_index);
        fprintf(file, "% 6d (% 6d) [% 7d/% 7d/% 7d/% 7d] % 7d % 7d %s\r\n",
                dump->count, dump->active_count, dump->garbage_count, dump->dead_count,
                dump->outside_map_count, dump->at_rest_count, dump->maximum_size, dump->total_size, name);
    }
    fprintf(file, "\n");

    if ( overflow_count > 0 )
        fprintf(file, "WARNING: overflowed MAXIMUM_DUMPS (%d), this dump does not include %d objects that would not fit!\n",
                MAXIMUM_DUMPS, overflow_count);
    fprintf(file, "\n");
    fclose(file);
}
