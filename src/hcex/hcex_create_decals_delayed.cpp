/* hcex_create_decals_delayed @0x823DDEB8 — drain the hcexCreateDecals queue built up over the frame:
 * for each queued decal, re-probe the world surface directly under it (scnSCENE::GetClosestPoint_EXT,
 * within 0.2 units) to get a fresh face normal, build a scrSCORCH_INI scorch/decal descriptor from it
 * (sharing the decal's name string, normalizing the probed normal), and hand it to the active
 * scrFAMILY_BASE scorch/decal manager via Add(). Skips any decal whose probe finds no nearby surface.
 * Clears the queue and closes the "create_decals_delayed" PIX event when done.
 *
 * deviation: the decompiler shows `scrSCORCH_INI`'s embedded `cdtInfo` member and the separate
 * `cdtREFINE` temporary as overlapping/reinterpreted raw stack slots (an artifact of PPC stack-slot
 * reuse across their non-overlapping lifetimes); confirmed via disasm (the GetFaceNormal `this` and
 * the Add() 2nd argument are the same base address offset by cdtInfo's/normal's real field offsets)
 * that they are, in fact, exactly `ini.cdtInfo` and `ini.normal`. Reconstructed here as the two plain
 * named locals (`ini`, `refine`) that produced that layout. cdtREFINE's own ctor semantics are not
 * decoded (it isn't one of this batch's targets); its raw `stateObjUsrIncl` bit-pattern argument is
 * reproduced verbatim from disasm. */

#include "../headers/hcex/hcex_create_decals_delayed_boundary.h"

void hcex_create_decals_delayed(void)
{
    osPIXBeginEvent("create_decals_delayed");

    for ( int i = 0; i < hcexCreateDecals.nElem; ++i )
    {
        HCEX_DECAL *decal = &hcexCreateDecals.pData[i];

        scrSCORCH_INI ini;
        scrSCORCH_INI_ctor(&ini);

        cdtREFINE refine;
        cdtREFINE_ctor(&refine, /*flags*/ 1, /*layerMask*/ 0,
                HCEX_DECAL_REFINE_STATE_OBJ_USR_INCL, /*pInstSelf*/ 0);

        if ( scnSCENE_GetClosestPoint_EXT(gsScenePtr, &decal->pos, 0.2f, &refine, &ini.cdtInfo) )
        {
            dsTSTRING_assign(&ini.nameClass, &decal->name);

            cdtINFO_GetFaceNormal(&ini.cdtInfo, &ini.normal);
            m3dNormalize(&ini.normal);
            ini.pAtkDisp = 0;

            scrFAMILY_BASE_Add(scrFamily, &ini);

            dsTSTRING_dtor(&ini.nameClass);
        }

        cdtREFINE_dtor(&refine);
    }

    dsVECTOR_HCEX_DECAL_8__Clear(&hcexCreateDecals);
    osPIXEndEvent();
}
