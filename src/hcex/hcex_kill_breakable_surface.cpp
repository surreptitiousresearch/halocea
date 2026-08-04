/* hcex_kill_breakable_surface @0x823B20C8 — tear down the ws-engine scene instance that visually
 * represents a Blam breakable surface once it has been broken. Looks the surface up by (bsp, idx) in the
 * sorted hcexBreakableSurfaces vector; if it has a live scene instance with an entity, invokes the entity's
 * "Destroy" SSL function (lazily interning the function name on first use) and releases the returned ref.
 *
 * Pure ws/SSL bridge glue: the container, scene-instance, SSL object/ref and ent/anim types are boundaries
 * (see hcex_kill_breakable_surface_boundary.h). */

#include "../headers/hcex/hcex_kill_breakable_surface_boundary.h"

extern "C" void hcex_kill_breakable_surface(int bsp, int idx)
{
    hcexBREAKABLE_SURF key;
    dsCMP cmp = 0;
    int found;

    key.bsp = bsp;
    key.idx = idx;
    scnINST_CREATE_DATA_ctor(&key.cd);

    found = dsVECTOR_BSURF_FindSorted(&hcexBreakableSurfaces, &key, &cmp);
    if ( found >= 0 )
    {
        animINST *inst = dsVECTOR_BSURF_index(&hcexBreakableSurfaces, found)->pInst;
        if ( inst && inst->pEnt )
        {
            sslERROR  *ssl_object;
            sslOBJ_REF self;
            sslERROR result;

            if ( (hcex_destroy_strid_initialized & 1) == 0 )
            {
                hcex_destroy_strid_initialized |= 1u;
                dsSTRID_init(&fnDestroy, "Destroy", 0);
            }

            ssl_object = (sslERROR *)entENTITY_get_sslObject(inst->pEnt);
            sslOBJ_REF_ctor(&self, 0);
            sslOBJ_REF_CallFunc(&result, ssl_object, &fnDestroy, 0, 0, 0, &self);

            /* release the sret sslERROR's desc string buffer: dec refcount, free at zero
             * (CallFunc returns sslERROR by value; the decompiler's --*v10 is the desc
             * dsTSTRING_flat buffer refcount release). */
            if ( --result.desc.pBuffer->refCount == 0 )
                dlFree(result.desc.pBuffer);
        }
    }

    scnINST_CREATE_DATA_dtor(&key.cd);
}
