/* store_parameter_in_exposed @0x83811CF0 — write a new value into a registered exposed parameter by name:
 * linear scan of g_exposed_params (inline strcmp in the compiled code) and, on a hit, a 4-byte store
 * through the parameter's live-storage pointer. Returns whether the name was found. */

#include <stdint.h>
#include "headers/exposed_parameter.h"

extern int strcmp(const char *s1, const char *s2);

uint8_t store_parameter_in_exposed(const char *query, void *value)
{
    for ( int i = 0; i < g_num_exposed_params; i++ )
    {
        if ( !strcmp(query, g_exposed_params[i].parameter_name) )
        {
            *g_exposed_params[i].___u2.integer_param = *(int *)value;
            return 1;
        }
    }
    return 0;
}
