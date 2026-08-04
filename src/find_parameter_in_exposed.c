/* find_parameter_in_exposed @0x83811C18 — look up a registered exposed parameter by name (inline strcmp in
 * the compiled code) and, when found, optionally read its live 4-byte value out. Returns whether the name
 * was found. */

#include <stdint.h>
#include "headers/exposed_parameter.h"

extern int strcmp(const char *s1, const char *s2);

uint8_t find_parameter_in_exposed(const char *query, int *value)
{
    for ( int i = 0; i < g_num_exposed_params; i++ )
    {
        if ( !strcmp(query, g_exposed_params[i].parameter_name) )
        {
            if ( value )
                *value = *g_exposed_params[i].___u2.integer_param;
            return 1;
        }
    }
    return 0;
}
