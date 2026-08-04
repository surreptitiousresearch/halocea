/* tag_data_get_pointer @0x83700B10 — return a pointer into a tag_data blob at the given byte offset. */

#include "headers/tag_data.h"

char *tag_data_get_pointer(const tag_data *data, int offset, int size)
{
    return (char *)data->address + offset;
}
