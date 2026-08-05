#include "../../headers/ws/strm/strmRES_TEX_MANAGER.h"
#include "../../headers/ws/strm/strmRESOURCE_PACK_S3D.h"
#include "../../headers/ws/strm/strm_boundary.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/ws/txm/txmTEXTURE.h"

extern int osLockedIncrement(int *value); // boundary — os subsystem, see os_boundary.h

// strmRES_TEX_MANAGER::UpdateResource2 @ 0x82671098
// Decide and apply this frame's mip-range action for `strmResource` against the
// (totalMem, memHi, memMid, memLo, rest) budget pointers.
//   - RES_STATE_CANCEL: flag notEnoughMem, log the shortfall to the strm-mem perf counter, and
//     drop back to idle.
//   - "static" streaming behavior (2) already at minMip: refresh the accumulated stats and (if
//     idle with no pending request) RemoveQuery, then report DO_NOTHING.
//   - streaming behavior >= 3 is unreachable (asserts and skips the mip-range recompute below,
//     leaving checkMip at its default of reqMip).
//   - otherwise: run Subupdate to find the mip level the memory budget allows (starting from
//     curMip when behavior == 2 ["static"], reqMip otherwise), then either raise toward it via
//     AddQuery (UPLOAD_MIPS) or lower toward it via UnloadResource (REMOVE_MIPS).
// Finally, debug-checks the texture, optionally dumps per-texture stats to tex_stat_na.csv when
// the `d2` debug trigger is set, and resets the max-mip accumulation counter.
int strmRES_TEX_MANAGER::UpdateResource2(strmRESOURCE_TEXTURE *strmResource, int *totalMem,
                                          int *memHi, int *memMid, int *memLo, int *rest)
{
    txmTEXTURE *ownerTex = strmResource->ownerTex;

    if (strmResource->state == strmRESOURCE::RES_STATE_CANCEL) {
        notEnoughMem = true;
        // DEVIATION: the decompiler builds a spurious 64-bit pair from an unrelated stack
        // address and CalcNeededMemSize()'s result before calling apCOUNTER::AddValue; the real
        // signature is AddValue(int unitIndex, float value) (mangled
        // ?AddValue@apCOUNTER@@QAAXHM@Z), so this is just "log the needed memory, in MB, to unit 2".
        cnt_strm_mem.AddValue(2, (float)strmResource->CalcNeededMemSize() * 0.00000095367432f);
        strmResource->SetState(strmRESOURCE::RES_STATE_IDLE);
    }

    int streamingBehavior = strmResource->GetStreamingBehavior();
    if (streamingBehavior == 2 && strmResource->curMip <= strmResource->minMip) {
        strmResource->ResetMaxMipCounter();
        if (strmResource->state == strmRESOURCE::RES_STATE_IN_QUERY && !strmResource->reqMip)
            strmResource->RemoveQuery();
        return strmRESOURCE_TEXTURE::DO_NOTHING;
    }

    int reqMip = strmResource->reqMip;
    int curMip = strmResource->curMip;
    int action = strmRESOURCE_TEXTURE::DO_NOTHING;
    int checkMip = reqMip; // default mip to re-check, overridden below for behavior == 2

    _DbgCheckTex(ownerTex);

    if (streamingBehavior >= 3) {
        if (!IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash("0",
                "D:\\Projects\\code\\common\\src.sys\\drv\\strm_res_tex_mng.cpp", 419,
                empty_string);
        // Unreachable in a well-formed build: falls straight through to the curMip/reqMip
        // comparison below with checkMip still at its default (reqMip) — Subupdate is skipped.
    } else {
        if (streamingBehavior == 2)
            checkMip = curMip;
        Subupdate(strmResource, totalMem, memHi, memMid, memLo, rest, &checkMip);
        reqMip = checkMip;
    }

    if (curMip == reqMip) {
        action = strmRESOURCE_TEXTURE::DO_NOTHING;
    } else if (curMip < reqMip) {
        int prevCurMip = strmResource->curMip;
        strmResource->reqMip = reqMip;
        if (reqMip > prevCurMip) {
            if (strmResource->AddQuery()) { // strmRESOURCE_PACK_S3D<strmRESOURCE,strmRESOURCE_TEXTURE>::AddQuery
                osLockedIncrement(&ownerTex->refCount);
                action = strmRESOURCE_TEXTURE::UPLOAD_MIPS;
            }
        }
    } else if (UnloadResource(strmResource, reqMip)) {
        action = strmRESOURCE_TEXTURE::REMOVE_MIPS;
    }

    _DbgCheckTex(ownerTex);

    if (d2.base.value) {
        // DEVIATION: the trailing 7 %i arguments (v30..v36 in the decompile) are never assigned
        // anywhere in the function — they are read uninitialized in the compiled binary too, so
        // they're reproduced here as genuinely uninitialized locals rather than invented values.
        int garbage0, garbage1, garbage2, garbage3, garbage4, garbage5, garbage6;
        _apForceLog("tex_stat_na.csv", "%s(%i), %ix%i, %i, %i, %f, %i, %i, %i, %i, %i,%i, %i\n",
            ownerTex->name.CStr(), (int)ownerTex->uniqueID, ownerTex->GetSizeX(), ownerTex->GetSizeY(),
            0, strmResource->matPriority, (double)strmResource->accDataVector.lastData.dist2.x,
            garbage0, garbage1, garbage2, garbage3, garbage4, garbage5, garbage6);
    }

    strmResource->ResetMaxMipCounter();
    return action;
}
