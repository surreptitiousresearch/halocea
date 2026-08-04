/* hcex_lipsync_term @0x823DCE48 — drop a Blam object id's lip-sync state. Looks the id up in the sorted
 * hcexLipSyncList (dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>), destroys the mdlLIP_SYNCER if present
 * (virtual dtor, deleting flag 1), and erases the entry. The dsVECTOR template ops and mdlLIP_SYNCER
 * are ws-engine C++ (extern boundary).
 *
 * Deviation: the decompiler modeled eleven junk integer params (a2..a12) from a misread stack frame;
 * only r3 (id) is real. a12 is reused as the sorted-find key local. */

#include "../headers/ds_vector_generic.h"
#include "../headers/lipsync_pair.h"

extern ds_vector_generic hcexLipSyncList;

extern int           dsVECTOR_lipsync_FindSorted(ds_vector_generic *vector, const int *key, void *comparator);
extern lipsync_pair *dsVECTOR_lipsync_index(ds_vector_generic *vector, int position);
extern void          dsVECTOR_lipsync_Erase(ds_vector_generic *vector, int position, int count);

void hcex_lipsync_term(int id)
{
    int key = id;
    char comparator = 0;  /* dsCMP — zero-initialized stateless comparator */

    int index = dsVECTOR_lipsync_FindSorted(&hcexLipSyncList, &key, &comparator);
    if ( index == -1 )
        return;

    mdlLIP_SYNCER *syncer = dsVECTOR_lipsync_index(&hcexLipSyncList, index)->val;
    if ( syncer )
    {
        /* virtual destructor at vtable slot 0; flag 1 = delete the object */
        (*(void (**)(mdlLIP_SYNCER *, int))(*(void **)syncer))(syncer, 1);
    }
    dsVECTOR_lipsync_Erase(&hcexLipSyncList, index, 1);
}
