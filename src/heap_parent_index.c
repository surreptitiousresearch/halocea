#include <stdint.h>
int16_t heap_parent_index(int16_t heap_index)
{
    return (short)((heap_index - 1) >> 1);
}
