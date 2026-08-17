/* metrics_timestamp_get @0x83818D10 */
#include "headers/metrics_globals.h"

char * metrics_timestamp_get(void)
{
    return metrics_globals.metrics_timestamp;
}
