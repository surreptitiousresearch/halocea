/* unpack_parameter_set_from_transmission @0x83811EE8 — render a received packed parameter set into
 * g_parameters_string as "name value\n" lines, one per registered exposed parameter (4-byte packed values,
 * in registration order): integers via "%d\n", floats via "%f\n" (disasm-confirmed formats; the compiled
 * inline strlen is written as a loop as shipped).
 *
 * DEVIATION (bug class 9): the decompiler mangled the float sprintf into a HIDWORD/COERCE pun; disasm
 * shows a plain "%f\n" with the float promoted to double in the varargs slot. */

#include "headers/exposed_parameter.h"
#include "headers/packed_parameter.h"
#include <string.h>
#include "headers/blam_data_globals.h"

extern int sprintf_0(char *string, const char *format, ...);

void unpack_parameter_set_from_transmission(packed_parameter *params)
{
    char *cursor = g_parameters_string;

    for ( int i = 0; i < g_num_exposed_params; i++, params++ )
    {
        const char *parameter_name = g_exposed_params[i].parameter_name;
        size_t name_length = 0;
        while ( parameter_name[name_length] )
            name_length++;

        memcpy(cursor, parameter_name, name_length);
        cursor[name_length] = ' ';
        cursor += name_length + 1;

        int written;
        if ( g_exposed_params[i].type == INTEGER )
            written = sprintf_0(cursor, "%d\n", params->integer_param);
        else
            written = sprintf_0(cursor, "%f\n", (double)params->real_param);
        cursor += written;
    }
}
