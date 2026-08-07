/* HCEX_INIT_STAT::~HCEX_INIT_STAT @0x823CEB94 — if a custom sound backend is installed, invoke
 * its deleting destructor (freeMemory=1) via the snd::SYSTEM_CUSTOM vtable slot, then clear the
 * global pointer. */

#include "../../headers/hcex/HCEX_INIT_STAT.h"

extern snd::SYSTEM_CUSTOM *snd_SystemCustom; // boundary global — `snd::SystemCustom`

HCEX_INIT_STAT::~HCEX_INIT_STAT()
{
    if ( snd_SystemCustom )
    {
        snd::SYSTEM_CUSTOM_vtbl *vftable = *reinterpret_cast<snd::SYSTEM_CUSTOM_vtbl **>(snd_SystemCustom);
        vftable->dtr_SYSTEM_CUSTOM(snd_SystemCustom, 1);
    }
    snd_SystemCustom = nullptr;
}
