/* hcexPARTICLE_SYS::PushDelayedCDT @0x823CEF88 (overrides gsPARTICLE_SYS::PushDelayedCDT) — run
 * the base engine's CDT push, then kick the global partJobCdt job on the three CDT thread masks
 * (2, 4, 16). */

#include "../../headers/hcex/hcexPARTICLE_SYS.h"
#include "../../headers/ws/part/partJOB_CDT.h"

void hcexPARTICLE_SYS::PushDelayedCDT()
{
    gsPARTICLE_SYS::PushDelayedCDT();
    partJobCdt.Start(2);
    partJobCdt.Start(4);
    partJobCdt.Start(16);
}
