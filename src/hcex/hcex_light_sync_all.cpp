/* hcex_light_sync_all @0x823DE0B8 — drive hcex_light_sync over every bridged Blam light in the
 * sorted hcexLights vector, bracketed by a "light_sync" PIX profiler event. */

#include "../headers/hcex/hcex_light_sync_boundary.h"

extern void hcex_light_sync(HCEX_LIGHT *light);

void hcex_light_sync_all(void)
{
    osPIXBeginEvent("light_sync");
    for ( int i = 0; i < hcexLights.nElem; ++i )
        hcex_light_sync(&hcexLights.pData[i].val);
    osPIXEndEvent();
}
