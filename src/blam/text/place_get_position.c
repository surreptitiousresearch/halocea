/* place_get_position @0x837464B0 — returns a statistic_buffer row's raw place ranking, masking off the
 * high "tied" flag bit set by populate_statistic_buffer (see get_place_string.c for the same mask used
 * elsewhere). */

#include "headers/statistic_buffer.h"

int place_get_position(statistic_buffer *buffer)
{
    return buffer->place & 0x7FFFFFFF;
}
