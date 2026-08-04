/* message_delta_parameters_protocol_shutdown @0x83811988 — DEVIATION: the decompiler's base pointer
 * "&symbol_buffer[16380]" is a mis-resolved symbol; disasm_range(0x83811988,0x838119E4) shows the real base
 * is `&g_exposed_params[0] - 12` (a pre-decrement trick feeding the `stwu ...,0xC(r30)` auto-increment
 * addressing), and each loop iteration frees and clears g_exposed_params[i].parameter_name — not some
 * unrelated "value" field at a +3-DWORD offset as the raw pseudocode's cast suggested. */

#include "headers/exposed_parameter.h"
#include "headers/blam_data_globals.h"

extern void dlFree(void *ptr);

void message_delta_parameters_protocol_shutdown(void)
{
    if ( g_protocol_changeover_enabled == 1 )
    {
        for ( int i = 0; i < g_num_exposed_params; i++ )
        {
            dlFree((void *)g_exposed_params[i].parameter_name);
            g_exposed_params[i].parameter_name = 0;
        }
        g_num_exposed_params = 0;
    }
}
