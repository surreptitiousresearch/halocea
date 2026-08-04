/* object_dump_write @0x836EE5F4 — write one object-memory-dump row to a text stream:
 * resolve the row's name (tag name when keyed by a definition index, else the object-type name,
 * else "unknown") and print the per-group counts and sizes.
 * DEVIATION: the decompiler lost the name assignment and the maximum_size/total_size fprintf args;
 * recovered from disassembly — args are maximum_size (+0x6) then total_size (+0x8) then name. */

#include <stdint.h>
#include "headers/dump_datum.h"

struct _iobuf;

extern char *tag_get_name(int16_t tag_index);
extern const char * object_type_get_name(int16_t object_type);
extern int fprintf(struct _iobuf *stream, const char *format, ...);

void object_dump_write(struct _iobuf *stream, dump_datum *dump)
{
    const char *name = "unknown";
    if ( dump->definition_index == -1 )
    {
        if ( (unsigned __int16)dump->object_type != 0xFFFF )
            name = object_type_get_name(dump->object_type);
    }
    else
    {
        name = tag_get_name(dump->definition_index);
    }
    fprintf(
        stream,
        "% 6d (% 6d) [% 7d/% 7d/% 7d/% 7d] % 7d % 7d %s\r\n",
        dump->count,
        dump->active_count,
        dump->garbage_count,
        dump->dead_count,
        dump->outside_map_count,
        dump->at_rest_count,
        dump->maximum_size,
        dump->total_size,
        name);
}
