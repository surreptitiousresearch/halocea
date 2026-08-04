/* is_place_tied @0x83746498 — tests statistic_buffer::place's "tied" flag (top bit, confirmed via
 * decompiler: `return buffer->place < 0;`). */

#include <stdint.h>
#include "headers/statistic_buffer.h"

uint8_t is_place_tied(statistic_buffer *buffer)
{
    return buffer->place < 0;
}
