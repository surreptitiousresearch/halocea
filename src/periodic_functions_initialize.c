/* periodic_functions_initialize @0x837B7700 */
#include <stdint.h>
#include "headers/blam_data_globals.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void periodic_function_build_table(int16_t function_type, uint8_t *integer_table);
extern void transition_function_build_table(int16_t function_type, uint8_t *integer_table);
extern uint32_t *get_global_random_seed_address(void);

extern uint8_t  periodic_functions_built_flag;

void periodic_functions_initialize(void)
{
    int16_t function_type;

    periodic_functions_built_flag = 1;
    *get_global_random_seed_address() = 552859232;

    for (function_type = 0; function_type < 12; function_type = (int16_t)(function_type + 1))
    {
        uint8_t *table = dlMalloc(0x400u,
                             "D:\\Projects\\code\\HCEX\\sources\\math\\periodic_functions.c", 0x4Eu);
        periodic_function_tables[function_type] = table;
        if (table)
            periodic_function_build_table(function_type, table);
        else
            periodic_functions_built_flag = 0;
    }

    for (function_type = 0; function_type < 6; function_type = (int16_t)(function_type + 1))
    {
        uint8_t *table = dlMalloc(0x400u,
                             "D:\\Projects\\code\\HCEX\\sources\\math\\periodic_functions.c", 0x60u);
        transition_function_tables[function_type] = table;
        if (table)
            transition_function_build_table(function_type, table);
        else
            periodic_functions_built_flag = 0;
    }
}
