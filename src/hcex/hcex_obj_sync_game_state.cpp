/* hcex_obj_sync_game_state(void) @0x823EDE28 — zero-arg overload: drive hcex_obj_sync_game_state_obj
 * (the per-object worker, see hcex_obj_sync_game_state_obj.c) over every tracked HCEX_OBJ whose
 * animated instance still has a live ws-engine entity, bracketed by a "hcex_obj_sync_game_state" PIX
 * profiler event. */

#include "../headers/hcex/hcex_obj_sync_game_state_boundary.h"

extern void hcex_obj_sync_game_state_obj(HCEX_OBJ *obj);

void hcex_obj_sync_game_state(void)
{
    osPIXBeginEvent("hcex_obj_sync_game_state");

    for ( int i = 0; i < hcexObjects.nElem; ++i )
    {
        HCEX_OBJ *obj = &hcexObjects.pData[i];
        if ( obj->pInst && obj->pInst->pEnt )
            hcex_obj_sync_game_state_obj(obj);
    }

    osPIXEndEvent();
}
