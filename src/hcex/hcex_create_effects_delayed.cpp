/* hcex_create_effects_delayed @0x823DE768 — drain the hcexCreateEffects queue built up over the frame:
 * for each queued named effect, spawn its driving ws-engine entity at the first placed location
 * (hcex_create_effect_impl), then — if that succeeded — feed every placed location's position/direction
 * to the new instance via hcex_prepare_locations. Clears the queue is done by the caller's Clear below;
 * closes the "create_effects_delayed" PIX event when done.
 *
 * deviation: the decompiler reuses one stack array (`dsTSTRING<char> v11[23]`) for two unrelated
 * purposes across the function's lifetime — first as the shared effect-name string handed to
 * hcex_create_effect_impl (which itself releases that string's reference before returning, per its own
 * source), then, inside the per-location loop, as scratch storage for a throwaway one-element
 * `const char*[1]` array satisfying hcex_prepare_locations' `names` parameter. Reconstructed here as two
 * separate, correctly-typed locals (`name`, `loc_name`). */

#include "../headers/hcex/hcex_create_effect_boundary.h"

extern void dsVECTOR_HCEX_EFFECT_8__Clear(dsVECTOR_EFF *v);
extern void osPIXBeginEvent(const char *string);
extern void osPIXEndEvent(void);

void hcex_create_effects_delayed(void)
{
    osPIXBeginEvent("create_effects_delayed");

    for ( int i = 0; i < hcexCreateEffects.nElem; ++i )
    {
        HCEX_EFFECT *effect = &hcexCreateEffects.pData[i];
        HCEX_EFFECT_LOC *locations = (HCEX_EFFECT_LOC *)effect->locations.pData;

        dsTSTRING_flat name;
        name.pBuffer = effect->hcexName.pBuffer;
        ++name.pBuffer->refCount;

        /* hcex_create_effect_impl consumes (and releases) `name`; spawn at the first placed location. */
        entENTITY *effect_impl = hcex_create_effect_impl(&name, -1, &locations[0].pos);

        if ( effect_impl )
        {
            for ( int j = 0; j < effect->locations.nElem; ++j )
            {
                HCEX_EFFECT_LOC *loc = &locations[j];
                const char *loc_name = loc->name.pBuffer->str;
                hcex_prepare_locations(effect_impl->pInst, &loc->pos, &loc->vec, &loc_name, 1);
            }
        }
    }

    dsVECTOR_HCEX_EFFECT_8__Clear(&hcexCreateEffects);
    osPIXEndEvent();
}
