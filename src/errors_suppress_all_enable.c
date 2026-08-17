/* errors_suppress_all_enable @0x83764B50 */
#include <stdint.h>
#include "headers/error_global_data.h"

void errors_suppress_all_enable(uint8_t suppression)
{
    error_globals.suppress_all = suppression;
}
