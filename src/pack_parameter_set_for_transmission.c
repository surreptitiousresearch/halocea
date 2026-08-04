/* pack_parameter_set_for_transmission @0x83811DC4 — snapshots every exposed (runtime-tunable) real
 * parameter's live value into a packed buffer for network transmission.
 *
 * DEVIATION: the binary unrolls this copy loop ×4 (a head loop of groups of four plus a remainder
 * loop); reproduced here as a single equivalent loop. */

#include "headers/packed_parameter.h"
#include "headers/exposed_parameter.h"

void pack_parameter_set_for_transmission(packed_parameter *params)
{
    for (int i = 0; i < g_num_exposed_params; ++i)
        params[i].real_param = *(float *)g_exposed_params[i].___u2.location;
}
