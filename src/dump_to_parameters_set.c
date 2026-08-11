/* dump_to_parameters_set @0x83811B08 — serialize all exposed tuning parameters into the g_parameters_string
 * text buffer, one "<name> <value>\n" line each (integer values via %d when type==1, else float via %f),
 * NUL-terminating the result. Used to snapshot the parameter set for network transmission.
 * The 12-byte record is DB type `exposed_parameter`: { char *parameter_name; parameter_type type;
 * union ___u2 { void *location; float *real_param; int *integer_param; }; }. */

#include "headers/exposed_parameter.h"
#include "headers/blam_data_globals.h"

extern int sprintf_0(char *string, const char *format, ...);

void dump_to_parameters_set(void)
{
    int offset = 0;
    for ( int i = 0; i < g_num_exposed_params; ++i )
    {
        char *out = &g_parameters_string[offset];
        int length;
        if ( g_exposed_params[i].type == INTEGER )
            length = sprintf_0(out, "%s %d\n", g_exposed_params[i].parameter_name, *g_exposed_params[i].___u2.integer_param);
        else
            length = sprintf_0(out, "%s %f\n", g_exposed_params[i].parameter_name, *g_exposed_params[i].___u2.real_param);
        offset += length;
    }
    g_parameters_string[offset] = 0;
}
