#include "../../headers/ws/prop/instCONSTRUCTOR_MNG.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/wb/dbgVAR_boundary.h"
#include "../../apCL.h"
#include <string.h>

// wb-subsystem debug toggle gating the "remove objects not touched by the new preset" pass
// (?dbg_del@@3Udbg_del@@... aliased through dbgVAR_SIMPLE<bool,1>). boundary global.
extern dbgVAR_SIMPLE<bool, 1> dbg_del;

// instCONSTRUCTOR_MNG::Construct(const dsTSTRING<char>&, animINST*, bool) — 0x826E3FD0
//
// Build/apply the named preset onto `pInst`: mark every currently-geometric object as
// "modifier-cache stale" (state bit 0x8), rebuild the object hierarchy through the preset's
// root node (ConstructInst, which fills a per-object "touched" flag vector), then — only in
// debug builds with `dbg_del` enabled, and only when not in skin-swap mode and the caller
// allows removal — delete every object the new preset did not touch (unless it is a
// still-visible, non-"visibility_hidden" affixed model with a modifier state that forbids
// pruning). Finishes by re-running LOD term/init on the instance.
void instCONSTRUCTOR_MNG::Construct(const dsTSTRING<char> &presetName, animINST *pInst, bool canRemove)
{
    if (!presetName.pBuffer->strLen)
        return;

    instCONSTRUCTOR_NODE *preset = GetPreset(presetName);
    if (!preset)
        return;

    instCONSTRUCTOR_NODE_RELATIONS *relations = GetRelations(presetName);

    for (int i = 0; i < pInst->nObj; ++i)
    {
        objOBJ *obj = pInst->ppObjList[i];
        if (obj && obj->pGeom)
            obj->stateProc |= 8u;
    }

    // Per-object "did the new preset touch this index" flags, sized to the instance's current
    // object count.
    dsVECTOR<unsigned char, 8> touched;
    touched.__cl.file = "D:\\Projects\\code\\common\\src.sys\\ws_shared\\ws_prop_inst_constructor.cpp";
    touched.__cl.line = 411;
    // Deviation from decompiler: it rendered Resize's own internal "idx>=0 && idx<nElem"
    // STRONG_ASSERT2 bounds check inline (ds_vector.hpp:567) — that assert is part of Resize's
    // own (boundary, not reversed) implementation, not Construct's logic, so it is left implicit
    // in this call rather than hand-reproduced.
    touched.Resize(pInst->nObj);
    memset(touched.pData, 0, touched.nElem);

    preset->ConstructInst(pInst, relations, touched);

    if (dbg_del.value && !skinMode && canRemove)
    {
        // Deviation from decompiler: it tracked this single loop counter in two registers
        // (`i` indexing `touched`, `v11` indexing `ppObjList`) that are provably always equal
        // (both start at 0 and increment once per iteration) — collapsed to one index here.
        for (int i = 0; i < pInst->nObj; ++i)
        {
            if (!touched[i])
            {
                objOBJ *obj = pInst->ppObjList[i];
                if (obj)
                {
                    if (obj->pGeom && (obj->state & 8) == 0 && !obj->HaveAffix("visibility_hidden", 0))
                        pInst->DeleteObj(pInst->ppObjList[i]);
                }
            }
        }
    }

    pInst->LODTerm();
    pInst->LODInit();

    // Deviation from decompiler: it rendered `touched`'s teardown as an explicit
    // `dlFree(v21[0].pData)` — this is dsVECTOR<unsigned char,8>'s own (boundary, not reversed
    // separately) destructor, which frees `pData`. Reconstructed here as ordinary scope-exit
    // RAII (touched is a plain stack local) rather than a hand-rolled free, to avoid modeling
    // a double free against the destructor that already runs here.
}
