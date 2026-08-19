/* hs_get_function_documentation_string @0x8372FE80 */
#include <stdint.h>
#include "headers/hs_function_definition.h"

void hs_get_function_documentation_string(int16_t function_index, char *buffer)
{
    char *dst = buffer;
    const char *src = hs_function_table[function_index]->documentation;
    char c;
    do
    {
        c = *src++;
        *dst++ = c;
    } while ( c );
}
