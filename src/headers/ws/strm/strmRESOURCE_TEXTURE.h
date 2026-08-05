#pragma once
#include "strmRESOURCE_PACK_S3D.h"
#include "strmRES_TEX_SIZE_CALC.h"
#include "../ds/dsARRAY.h"
#include "../m3d/m2dV.h"
#include "../../pct_boundary.h"
// ws-engine strm (streaming): a streamed texture resource — tracks the current/requested mip
// range, per-frame view-distance statistics (accDataVector) used to decide how aggressively to
// stream mips in/out, and the packed-file plumbing inherited from strmRESOURCE_PACK_S3D.
// DB-verified layout (types_members strmRESOURCE_TEXTURE): base
// strmRESOURCE_PACK_S3D<strmRESOURCE,strmRESOURCE_TEXTURE>@0 (24B), header@24 (pctHEADER, 28B),
// maxMip@52, minMip@56, curMip@60, reqMip@64, viewMip@68, loadingMip@72, removingMip@76,
// skipMips@80, matPriority@84, dup_dvp@88, ownerTex@92, counter@96, lastBeh@100,
// accDataVector@104 (224B), mipAdd@328, sizeCalc@332 (16B) — size 348.

struct txmTEXTURE; // ../txm/txmTEXTURE.h

struct strmRESOURCE_TEXTURE : strmRESOURCE_PACK_S3D<strmRESOURCE, strmRESOURCE_TEXTURE> {
    // strmRESOURCE_TEXTURE::REQ_ACTION — DB-verified (types_enum_values
    // strmRESOURCE_TEXTURE::REQ_ACTION). The UpdateResource2 return value.
    enum REQ_ACTION {
        UPLOAD_MIPS = 0,
        REMOVE_MIPS = 1,
        DO_NOTHING  = 2,
    };

    // strmRESOURCE_TEXTURE::ACCUM_DATA — one mip-split's worth of accumulated per-frame view
    // statistics. DB-verified layout (types_members strmRESOURCE_TEXTURE::ACCUM_DATA) — size 32.
    struct ACCUM_DATA {
        m2dV mipCoef;   // 0x00 [x=min,y=max] observed mip-selection coefficient this period
        m2dV coefDist;  // 0x08 [x=min,y=max] observed coefficient-derived distance
        m2dV dist2;     // 0x10 [x=min,y=max] observed squared view distance
        int  splitCount; // 0x18 number of samples merged into this entry
        int  pixels;     // 0x1C accumulated on-screen pixel coverage
    };

    // strmRESOURCE_TEXTURE::ACCUM_DATA_VECTOR — a ring of per-mip-split ACCUM_DATA entries
    // (curData) plus the most recently finalized period (lastData). DB-verified layout
    // (types_members strmRESOURCE_TEXTURE::ACCUM_DATA_VECTOR): curData@0 (ds::ARRAY<ACCUM_DATA,6>,
    // 192B), lastData@192 (32B) — size 224.
    struct ACCUM_DATA_VECTOR {
        ds::ARRAY<ACCUM_DATA, 6> curData;  // 0x00
        ACCUM_DATA                lastData; // 0xC0

        // 0x82BF67F8 — fold every entry of curData into lastData (min/max coefficient and
        // distance bounds, summed pixel/split counts), then reset each curData entry to its
        // identity bounds ([100,-100] / [100000,-100000] / zero counters) ready for the next
        // accumulation period.
        void Merge();
    };

    pctHEADER header;     // 0x18
    int        maxMip;     // 0x34 coarsest mip index this texture ever streams down to
    int        minMip;     // 0x38 finest mip index this texture can request
    int        curMip;     // 0x3C mip index currently resident/uploaded
    int        reqMip;     // 0x40 mip index most recently requested by UpdateResource2
    int        viewMip;    // 0x44 mip index actually being rendered this frame
    int        loadingMip; // 0x48 mip index of the in-flight load, if any (0 = none)
    int        removingMip; // 0x4C mip index of the in-flight mip removal, if any
    int        skipMips;    // 0x50
    int        matPriority; // 0x54 material-authored streaming priority
    float      dup_dvp;      // 0x58
    txmTEXTURE *ownerTex;    // 0x5C the texture this streams data into
    int         counter;      // 0x60 ResetMaxMipCounter's period counter (mod 5)
    int         lastBeh;       // 0x64 last-observed GetStreamingBehavior() result
    ACCUM_DATA_VECTOR accDataVector; // 0x68
    int          mipAdd;              // 0x148
    strmRES_TEX_SIZE_CALC sizeCalc;   // 0x14C

    strmRESOURCE_TEXTURE();  // 0x82BF5760 — boundary, external to this batch
    ~strmRESOURCE_TEXTURE(); // boundary

    strmRESOURCE_TEXTURE &operator=(const strmRESOURCE_TEXTURE &that); // 0x8266F5B8

    int GetStreamingBehavior();  // vtbl+0x20 — boundary, external to this batch
    int GetCountMipsToRemove();  // vtbl+0x24 — boundary, external to this batch
    int CalcNeededMemSize();      // vtbl+0x30 — boundary, external to this batch

    // 0x82BF5E58 — cancel any outstanding streaming query for this resource, drop back to
    // RES_STATE_IDLE, and clear loadingMip. Always returns true.
    int RemoveQuery();

    // 0x82BF6B20 — every 5th call (counter % 5 == 0), fold this period's per-mip view statistics
    // into accDataVector.lastData via ACCUM_DATA_VECTOR::Merge.
    void ResetMaxMipCounter();

    // 0x82BF6BB8 — hardware-swizzled byte size for (sizeX, sizeY, mipCount) at this texture's
    // pixel format, memoized in `sizeCalc`.
    unsigned int CalcTexureSize(unsigned int sizeX, unsigned int sizeY, unsigned int mipCount);
};

// txm hardware texture-size helper (pct/txm boundary — external to this batch).
extern unsigned int txmCalcTexureSizeHW(unsigned int sizeX, unsigned int sizeY,
                                         unsigned int mipCount, int format);
