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
            {
                /* DEVIATION: the decompiler dropped the type discrimination — disasm
                 * @0x83811CE4 reads .type (offset 4), cmpwi ,1 (INTEGER): lwz/stw int copy,
                 * else lfs/stfs float copy @0x83811CD8. Bit-identical on PPC, but the branch
                 * is in the binary. Restored from disassembly. */
                if ( g_exposed_params[i].type == INTEGER )
                    *value = *g_exposed_params[i].___u2.integer_param;
                else
                    *(float *)value = *g_exposed_params[i].___u2.real_param;
            }
            return 1;
        }
    }
    return 0;
}
