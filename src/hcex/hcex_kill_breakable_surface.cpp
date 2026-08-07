/* hcex_kill_breakable_surface @0x823B20C8 — tear down the ws-engine scene instance that visually
 * represents a Blam breakable surface once it has been broken. Looks the surface up by (bsp, idx) in the
 * sorted hcexBreakableSurfaces vector; if it has a live scene instance with an entity, invokes the entity's
 * "Destroy" SSL function (lazily interning the function name on first use) and releases the returned ref.
 *
 * DEVIATION (2026-08-07): the surface slot, its create-data, the SSL object ref and the SSL error result
 * were previously flat plain-C restatements in this file's boundary header, driven through free-function
 * sret shims. They are the canonical ws-engine types now (odr_dup drain), so the C++ forms the binary
 * actually calls are spelled directly: `hcexBREAKABLE_SURF key;` constructs/destroys its embedded
 * scnINST_CREATE_DATA (the `bl scnINST_CREATE_DATA::scnINST_CREATE_DATA(void)` @0x823B20E8 and
 * `bl scnINST_CREATE_DATA::~scnINST_CREATE_DATA(void)` @0x823B21D8 — the folded/inlined
 * hcexBREAKABLE_SURF ctor and dtor, both at 0x823B16F8 / 0x823B1144), the caller ref is built as a
 * temporary directly into the by-value argument slot (`bl sslOBJ_REF::sslOBJ_REF(sslOBJECT *)`
 * @0x823B218C with no copy ctor and no matching dtor — MSVC has the callee destroy a by-value class
 * parameter), and the discarded sslERROR result's desc buffer is released by that temporary's
 * destructor at the end of the full expression, which is the `bl dlFree` @0x823B21D0. */

#include "../headers/hcex/hcex_kill_breakable_surface_boundary.h"

extern "C" void hcex_kill_breakable_surface(int bsp, int idx)
{
    hcexBREAKABLE_SURF key;
    dsCMP cmp;
    int found;

    key.bsp = bsp;
    key.idx = idx;

    found = hcexBreakableSurfaces.FindSorted(key, cmp);
    if ( found >= 0 )
    {
        animINST *inst = hcexBreakableSurfaces[found].pInst;
        if ( inst && inst->pEnt )
        {
            if ( (hcex_destroy_strid_initialized & 1) == 0 )
            {
                hcex_destroy_strid_initialized |= 1u;
                dsSTRID_init(&fnDestroy, "Destroy", 0);
            }

            entENTITY_get_sslObject(inst->pEnt)
                ->CallFunc(fnDestroy, 0, 0, 0, sslOBJ_REF((sslOBJECT *)0));
        }
    }
}
