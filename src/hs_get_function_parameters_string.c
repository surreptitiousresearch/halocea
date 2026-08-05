/* hs_get_function_parameters_string @0x8372FCEC — format a script function's call signature into `buffer`
 * as "(name ...)". If the function supplies an explicit parameters string it is appended verbatim;
 * otherwise each formal parameter is appended as " <typename>". The string always closes with ")".
 *
 * The binary inlines the literal/type appends as byte-copy (strcat-equivalent) loops and reaches the end
 * of the buffer with a NUL-scan before each append; this is written as strlen/strcat for readability. */

#include <stdint.h>
#include "headers/hs_function_definition.h"
#include "headers/blam_data_globals.h"

extern int sprintf_0(char *string, const char *format, ...);
extern unsigned int strlen(const char *string);
extern char *strcat(char *destination, const char *source);

void hs_get_function_parameters_string(int16_t function_index, char *buffer)
{
    const hs_function_definition *function = hs_function_table[function_index];

    sprintf_0(buffer, "(%s", function->name);

    if (function->parameters)
    {
        sprintf_0(buffer + strlen(buffer), " %s", function->parameters);
    }
    else if (function->formal_parameter_count > 0)
    {
        for (int16_t i = 0; i < function->formal_parameter_count; i++)
        {
            strcat(buffer, " <");
            strcat(buffer, hs_type_names[function->formal_parameters[i]]);
            strcat(buffer, ">");
        }
    }

    strcat(buffer, ")");
}
