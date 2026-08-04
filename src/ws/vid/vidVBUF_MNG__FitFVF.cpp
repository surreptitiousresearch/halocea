#include "../../headers/ws/vid/vidVBUF_MNG.h"
#include "../../headers/ws/vid/vidFVF_DESCR.h"

// Canonical strong-assert machinery (struct STRONG_ASSERT_DUMMY, IGNORE_STRONG_ASSERT,
// dsStrongAssertMessage). empty_string was the decompiler's char view of dsStrongAssertMessage.
#include "../../headers/ws/ds/ds_assert_boundary.h"

// 0x827B7890 -- ?FitFVF@vidVBUF_MNG@@...  vid_vbuf.cpp
// Find (or create) the cached FVF descriptor for a plain single-stream interleaved fvf. The
// requested key is (full = fvf, multistream = 0, interleaved = fvf, instanced = 0).
//
// The decompiler mis-modeled the single 64-bit `fvf` parameter (the disasm shows one 64-bit
// register `r4` and `cmpld` full-width compares against each descriptor's fvf fields) as an int
// plus register-pair halves; this reconstruction follows the disassembly.
//
// Lookup order: (1) 5-entry MRU cache -- on a hit, promote the descriptor to the front and return
// it; (2) 200-entry descriptor table -- on a hit, install it as the MRU front and return it;
// (3) first free descriptor-table slot -- initialize it for this fvf, build its declaration, and
// return it. Exhausting the table is a fatal assertion (vid_vbuf.cpp:178).
vidFVF_DESCR *vidVBUF_MNG::FitFVF(unsigned long long fvf)
{
    // (1) MRU cache scan
    for (int i = 0; i < 5; ++i)
    {
        vidFVF_DESCR *desc = this->fvfCacheLastUsed[i];
        if (desc
            && desc->fvf_full == fvf
            && desc->fvf_multistream == 0
            && desc->fvf_interleaved == fvf
            && desc->fvf_instanced == 0)
        {
            // Promote to front (swap slot i with slot 0).
            vidFVF_DESCR *front = this->fvfCacheLastUsed[0];
            this->fvfCacheLastUsed[i] = front;
            this->fvfCacheLastUsed[0] = desc;
            return desc;
        }
    }

    // (2) full descriptor-table scan
    for (int j = 0; j < 200; ++j)
    {
        vidFVF_DESCR *desc = this->fvfList[j];
        if (desc->sizeOfFVF
            && desc->fvf_full == fvf
            && desc->fvf_multistream == 0
            && desc->fvf_interleaved == fvf
            && desc->fvf_instanced == 0)
        {
            vidFVF_DESCR *front = this->fvfCacheLastUsed[0];
            this->fvfCacheLastUsed[1] = front;
            this->fvfCacheLastUsed[2] = front;
            this->fvfCacheLastUsed[3] = front;
            this->fvfCacheLastUsed[4] = front;
            this->fvfCacheLastUsed[0] = desc;
            return desc;
        }
    }

    // (3) allocate into the first free (uninitialized) descriptor slot
    for (int k = 0; k < 200; ++k)
    {
        vidFVF_DESCR *desc = this->fvfList[k];
        if (!desc->sizeOfFVF)
        {
            desc->Init(fvf, 0, fvf, 0);
            desc->CreateDecl();
            return desc;
        }
    }

    if (!IGNORE_STRONG_ASSERT)
        ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
            "0",
            "D:\\Projects\\code\\common\\src.sys\\drv\\video\\vid_vbuf.cpp", 178, dsStrongAssertMessage);
    return nullptr;
}
