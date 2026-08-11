/* tag_data_get_pointer @0x83700B10 — return a pointer into a tag_data blob at the given byte offset. */

#include <stdint.h>
#include "headers/tag_groups.h"

/* DEVIATION: returns `void *` per the DB decl @0x83700B10; the pre-B12b-2 spelling was `char *`,
 * which the callers relied on for bare pointer arithmetic. */
void *tag_data_get_pointer(const tag_data *data, int offset, int size)
{
    return (char *)data->address + offset;
}
