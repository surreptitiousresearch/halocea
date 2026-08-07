/* message_delta_parameters_protocol_expose_parameter @0x83811FD8 — register a runtime-tunable networking
 * parameter with the exposed-parameter system (only while the protocol-changeover config path is enabled).
 * The parameter's lookup key is "<type_name>::<parameter_name>" when a type name is given, otherwise just
 * "<parameter_name>". If that key is not already registered it is appended to g_exposed_params with its type
 * and live storage location. Finally the current config file is consulted for an override value, parsed as
 * "%d" for INTEGER parameters and "%f" otherwise, written straight into the parameter's storage.
 *
 * DEVIATION: the binary inlines the strcpy/strcat that build the key and (in the no-type path) copies a
 * dlStrdup'd name into the freshly allocated buffer; these are written as the equivalent library calls. */

#include <stdint.h>
#include <string.h>
#include "headers/exposed_parameter.h"
#include "headers/blam_data_globals.h"


extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern char *dlStrdup(char *ptr, const char *file, uint32_t line);
/* strcat declared by <string.h> */
/* strlen declared by <string.h> */
extern uint8_t find_parameter_in_exposed(const char *query, int *value);
extern int find_parameter_in_config(const char *query, const char *format, void *value);

void message_delta_parameters_protocol_expose_parameter(const char *type_name, const char *parameter_name,
        parameter_type type, void *location)
{
    static const char *const source_file =
            "D:\\Projects\\code\\HCEX\\sources\\networking\\message_delta_parameters_protocol.c";

    if ( g_protocol_changeover_enabled != 1 )
        return;

    char *name;
    if ( type_name )
    {
        name = dlMalloc(strlen(type_name) + strlen(parameter_name) + 3, source_file, 0xB9u);
        strcpy(name, type_name);
        strcat(name, "::");
        strcat(name, parameter_name);
    }
    else
    {
        name = dlMalloc(strlen(parameter_name) + 1, source_file, 0xC2u);
        /* dlStrdup only reads its source (returns a fresh writable copy); drop the caller's const view */
        strcpy(name, dlStrdup((char *)parameter_name, source_file, 0xC3u));
    }

    if ( !find_parameter_in_exposed(name, nullptr) )
    {
        int index = g_num_exposed_params++;
        g_exposed_params[index].parameter_name = name;
        g_exposed_params[index].type = type;
        g_exposed_params[index].___u2.location = location;
    }

    find_parameter_in_config(name, type == INTEGER ? "%d" : "%f", location);
}
