/* set_copy_error @0x83808500 */
#include <stdint.h>
#include "headers/simple_decompressor_definition.h"

void set_copy_error(int16_t flag)
{
    global_self->flags |= 1u << flag;
}
