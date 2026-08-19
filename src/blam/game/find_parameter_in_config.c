/* find_parameter_in_config @0x83811BB0 */
#include "headers/blam_data_globals.h"
extern char *strstr(const char *str1, const char *str2);
/* sscanf provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

int find_parameter_in_config(const char *query, const char *format, void *value)
{
    const char *found = strstr(g_parameters_string, query);

    if ( !found )
        return 0;

    char *space = strstr(found, " ");
    return sscanf(space + 1, format, value) > 0;
}
