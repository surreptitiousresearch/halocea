#include "headers/blam_data_globals.h"
/* message_delta_parameters_protocol_get @0x83811AF0 — return the current message-delta protocol number from
 * the global parameters. */


unsigned int message_delta_parameters_protocol_get(void)
{
    return g_parameters_protocol_number;
}
