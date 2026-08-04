/* hcex_start_cine @0x823BADE0 — begin an HCEX cinematic for the script that invoked it. Unless disabled by
 * the debug toggle, clears the per-cinematic hidden-actor/object bookkeeping and kicks off the cine process
 * for the parent script. When the dump or make-save debug toggles are on, records the parent script name
 * into the global debug cine-info record and initialises capture (once). hcex_get_parent_script names the
 * calling HaloScript's script; dsVECTOR / dsTSTRING_flat / DBG_CINE_INFO are ws-engine / bridge boundaries. */

#include "../headers/hcex/hcex_start_cine_boundary.h"   /* cine containers, DBG_CINE_INFO, dsTSTRING_flat */
#include "../headers/hcex/hcex_dbgvar_boundary.h"       /* dbg_hcex_DisableCine/DumpCine/CineMakeSave */

extern "C" void hcex_start_cine(void)
{
    const char *parent_script = hcex_get_parent_script();

    if ( !dbg_hcex_DisableCine.value )
    {
        dsVECTOR_tstring_Clear(&cineHiddenActorsTplNames);
        cineHiddenHcexObjKeys.nElem = 0;

        dsTSTRING_flat script;
        script.pBuffer = 0;
        dsTSTRING_UnsafeInit(&script, parent_script, -1, 0);
        hcex_cine_process_start(&script);
        if ( script.pBuffer->refCount-- == 1 )
            dlFree(script.pBuffer);
    }

    if ( dbg_hcex_DumpCine.value || dbg_hcex_CineMakeSave.value )
    {
        if ( !_dbgCineInfo.isInited )
        {
            dsTSTRING_flat script;
            script.pBuffer = 0;
            dsTSTRING_UnsafeInit(&script, parent_script, -1, 0);
            dsTSTRING_assign(&_dbgCineInfo.name, &script);
            if ( script.pBuffer->refCount-- == 1 )
                dlFree(script.pBuffer);
        }

        if ( !_dbgCineInfo.isInited && _dbgCineInfo.name.pBuffer->strLen )
            DBG_CINE_INFO_Init(&_dbgCineInfo);
    }
}
