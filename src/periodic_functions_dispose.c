/* periodic_functions_dispose @0x837B6DA0 */
#include "headers/blam_data_globals.h"
extern void dlFree(void *ptr);
extern unsigned char periodic_functions_built_flag;

void periodic_functions_dispose(void)
{
    if (periodic_functions_built_flag)
    {
        for (short i = 0; i < 12; i++)
            dlFree(periodic_function_tables[i]);
        for (short i = 0; i < 6; i++)
            dlFree(transition_function_tables[i]);
        periodic_functions_built_flag = 0;
    }
}
