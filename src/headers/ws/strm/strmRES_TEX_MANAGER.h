#pragma once
#include "strmRESOURCE_TEXTURE.h"
#include "../ds/dsVECTOR.h"
// ws-engine strm (streaming): the per-frame texture-streaming director. Owns every live streamed
// texture (`resources`), the mips-to-unload worklist (`unload`), and drives each resource's
// mip range toward its ideal (view-distance-driven) value within the available video-memory
// budget. DB-verified layout (types_members strmRES_TEX_MANAGER): memSizeReserved@0,
// notEnoughMem@4, miniZoomMode@5, autoMiniZoomOff@8, resources@12 (dsVECTOR<strmRESOURCE_TEXTURE*,
// 8>), unload@32 (same), resTexData@52 (dsVECTOR<strmRES_TEX_DATA,8>), preloadInfo@72
// (fioFILE_MEM*), strmMemCredit@76 — size 80.

struct fioFILE_MEM; // ../fio/fioFILE_MEM.h — boundary

typedef struct strmRES_TEX_MANAGER {
    // strmRES_TEX_MANAGER::strmRES_TEX_DATA — one entry of the priority-sorted texture-update
    // worklist. DB-verified layout (types_members strmRES_TEX_MANAGER::strmRES_TEX_DATA) — size 20.
    struct strmRES_TEX_DATA {
        int                 matPriority; // 0x00
        float                dist;        // 0x04
        int                  idRec;        // 0x08
        int                  pixels;        // 0x0C
        strmRESOURCE_TEXTURE *res;           // 0x10
    };

    int   memSizeReserved;  // 0x00
    bool   notEnoughMem;      // 0x04 set once UpdateResource2 sees a resource stuck in RES_STATE_CANCEL
    bool   miniZoomMode;       // 0x05
    unsigned char _pad0[2];     // 0x06 db-verified padding
    int    autoMiniZoomOff;     // 0x08
    dsVECTOR<strmRESOURCE_TEXTURE *, 8> resources; // 0x0C every live streamed texture
    dsVECTOR<strmRESOURCE_TEXTURE *, 8> unload;    // 0x20 pending UnloadResource worklist
    dsVECTOR<strmRES_TEX_DATA, 8>       resTexData; // 0x34
    fioFILE_MEM                         *preloadInfo; // 0x48
    int                                   strmMemCredit; // 0x4C

    strmRES_TEX_MANAGER();  // 0x82677370 — boundary, external to this batch
    ~strmRES_TEX_MANAGER(); // boundary

    void AddResource(strmRESOURCE_TEXTURE *tex); // boundary — external to this batch
    void DecreaseMemCredit(int amount);           // boundary — external to this batch

    // Body lives in the strm subsystem — boundary (called from hcex_change_render_mode). Drop the
    // whole streamed-texture worklist ahead of a render-mode swap.
    void Clear();                                 // boundary

    // ?mng@strmRES_TEX_MANAGER@@0V1@A — the streaming-texture manager singleton.
    static strmRES_TEX_MANAGER mng;               // boundary

    // 0x826718C0 — per-frame entry point for one texture: reserve video memory, mark the owning
    // texture as used-in-render, and drive its mip range via UpdateResource2 (spinning on
    // osSleep(0) while a load/removal is in flight, and swapping in the loaded surface once
    // RES_STATE_READY is reached).
    void NecessaryStreamTexture(txmTEXTURE *pTex);

    // 0x82670390 — debug-only consistency check (skipped once the texture has been released,
    // bit 28 of state.state): asserts the resident mip matches the loaded surface pointer
    // (lpSysSurfLowRes/lpSysSurfHiRes) and, for the hi-res case, that the GPU-reported texture
    // layout base offset matches memHandle->offset. `pTex` is actually a txmD3D_TEX* at runtime
    // (the fields touched — lpSysSurf/lpSysSurfLowRes/lpSysSurfHiRes/memHandle — live past the
    // plain txmTEXTURE layout); mangled as taking txmTEXTURE* per the base-class declaration.
    void _DbgCheckTex(txmTEXTURE *pTex);

private:
    // 0x82671098 — decide and apply this frame's mip-range action for `strmResource`: on
    // RES_STATE_CANCEL, flag notEnoughMem and drop back to idle; skip entirely if the streaming
    // behavior is "static" (2) and already at minMip; otherwise run Subupdate to find the mip
    // level the memory budget allows, then AddQuery (raise) or UnloadResource (lower) toward it.
    // Returns a strmRESOURCE_TEXTURE::REQ_ACTION.
    int UpdateResource2(strmRESOURCE_TEXTURE *strmResource, int *totalMem, int *memHi, int *memMid,
                         int *memLo, int *rest);

    // 0x8266EC20 — walk mip levels downward from *checkMip toward minMip, sized against the
    // (totalMem, memHi, memMid, memLo) budget pointers in turn; on the first mip whose
    // CalcTexureSize fits the corresponding budget, debit that budget and *totalMem and write the
    // accepted mip back through checkMip.
    int Subupdate(strmRESOURCE_TEXTURE *strmResource, int *totalMem, int *memHi, int *memMid,
                  int *memLo, int *rest, int *checkMip);

    // 0x82670FF8 — request dropping `strmResource` down to `mip`: in mini-zoom mode with the
    // resource already close enough (lastData.dist2.x < 5), just refresh curData[0] and mark the
    // texture used-in-render without touching state; otherwise cancel any in-flight query, record
    // removingMip, and enqueue it on `unload`. Fails (returns false) while a load is already in
    // flight (RES_STATE_LOADING).
    bool UnloadResource(strmRESOURCE_TEXTURE *strmResource, int mip);
} strmRES_TEX_MANAGER;
