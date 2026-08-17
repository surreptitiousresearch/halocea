/* get_copy_error_flags @0x83808528 */
#include "headers/simple_decompressor_definition.h"

unsigned int get_copy_error_flags(void)
{
    return global_self->flags;
}
