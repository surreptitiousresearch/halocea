/* heap_right_index @0x8381D078 */
#include <stdint.h>
int16_t heap_right_index(int16_t heap_index)
{
    return (short)(2 * (heap_index + 1));
}
