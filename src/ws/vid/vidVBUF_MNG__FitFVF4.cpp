#include "../../headers/ws/vid/vidVBUF_MNG.h"
#include "../../headers/ws/vid/vidFVF_DESCR.h"

// Canonical strong-assert machinery (struct STRONG_ASSERT_DUMMY, IGNORE_STRONG_ASSERT,
// empty_string). empty_string was the decompiler's char view of empty_string.
#include "../../headers/ws/ds/ds_assert_boundary.h"

// 0x827B7A88 -- ?FitFVF@vidVBUF_MNG@@...  vid_vbuf.cpp
// The four-mask overload of FitFVF: find (or create) the cached descriptor matching an explicit
// (full, multistream, interleaved, instanced) fvf key.
//
// NOTE ON ARITY: the decompiler renders this as taking a leading int plus register-pair halves
// (an apparent five-argument function). The disasm shows exactly four 64-bit register arguments
// (r4..r7) forwarded verbatim to vidFVF_DESCR::Init; the "fifth argument" is an aliasing artifact.
//
// The masks are first normalized by building a temporary descriptor and running its Init, then the
// normalized key is looked up with the same MRU-cache / descriptor-table / free-slot logic as the
// single-mask FitFVF. Exhausting the table is a fatal assertion (vid_vbuf.cpp:219).
vidFVF_DESCR *vidVBUF_MNG::FitFVF(unsigned long long fvf_full, unsigned long long fvf_multistream,
                                  unsigned long long fvf_interleaved, unsigned long long fvf_instanced)
{
    // Normalize the requested key through a temporary descriptor.
    vidFVF_DESCR key;
    key.Init(fvf_full, fvf_multistream, fvf_interleaved, fvf_instanced);

    // (1) MRU cache scan
    for (int i = 0; i < 5; ++i)
    {
        vidFVF_DESCR *desc = this->fvfCacheLastUsed[i];
        if (desc
            && desc->fvf_full == key.fvf_full
            && desc->fvf_multistream == key.fvf_multistream
            && desc->fvf_interleaved == key.fvf_interleaved
            && desc->fvf_instanced == key.fvf_instanced)
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
            && desc->fvf_full == key.fvf_full
            && desc->fvf_multistream == key.fvf_multistream
            && desc->fvf_interleaved == key.fvf_interleaved
            && desc->fvf_instanced == key.fvf_instanced)
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
            desc->Init(fvf_full, fvf_multistream, fvf_interleaved, fvf_instanced);
            desc->CreateDecl();
            return desc;
        }
    }

    if (!IGNORE_STRONG_ASSERT)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "0",
            "D:\\Projects\\code\\common\\src.sys\\drv\\video\\vid_vbuf.cpp", 219, empty_string);
    return nullptr;
}
