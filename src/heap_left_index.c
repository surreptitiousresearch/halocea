#include <stdint.h>
int16_t heap_left_index(int16_t heap_index)
{
    return (short)(2 * heap_index + 1);
}
