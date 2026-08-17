/* antennas_dispose @0x8380D320 */
#include "headers/blam_data_globals.h"

void antennas_dispose(void)
{
    if (antenna_data)
        antenna_data = 0;
}
