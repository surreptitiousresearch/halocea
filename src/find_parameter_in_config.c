#include "headers/blam_data_globals.h"
extern char *strstr(const char *str1, const char *str2);
extern int sscanf(const char *string, const char *format, ...);

int find_parameter_in_config(const char *query, const char *format, void *value)
{
    const char *found = strstr(g_parameters_string, query);

    if ( !found )
        return 0;

    char *space = strstr(found, " ");
    return sscanf(space + 1, format, value) > 0;
}
