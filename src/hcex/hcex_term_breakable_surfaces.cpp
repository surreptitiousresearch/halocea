/* hcex_term_breakable_surfaces @0x823B2608 — destroy the scene instances backing every breakable surface:
 * for each slot with a live instance, post a destroy message to its entity (or terminate the instance directly
 * if it has none), then null the slot. When `clear` is set, also release the vector's backing storage.
 *
 * hcex-bridge code; ws-engine container/message primitives and hcex_obj_term_inst are boundary externs.
 * Deviation: debug-only STRONG_ASSERT2 bounds checks elided. */

#include "../headers/animINST.h"
#include "../headers/hcex/hcex_surf_vector.h"

extern hcex_surf_vector hcexBreakableSurfaces;
extern struct msgSYSTEM msgSystem;

extern void msgSYSTEM_SendMsg(struct msgSYSTEM *system, entENTITY *entity, int message, void *a, void *b);
extern void hcex_obj_term_inst(animINST *instance);
extern void hcexBreakableSurfaces_Realloc(hcex_surf_vector *vector, int newSize);

void hcex_term_breakable_surfaces(int clear)
{
    for ( int i = 0; i < hcexBreakableSurfaces.nElem; ++i )
    {
        animINST *instance = hcexBreakableSurfaces.pData[i].pInst;
        if ( instance )
        {
            if ( instance->pEnt )
                msgSYSTEM_SendMsg(&msgSystem, instance->pEnt, 2, 0, 0);
            else
                hcex_obj_term_inst(instance);
            hcexBreakableSurfaces.pData[i].pInst = 0;
        }
    }

    if ( clear )
        hcexBreakableSurfaces_Realloc(&hcexBreakableSurfaces, 0);
}
