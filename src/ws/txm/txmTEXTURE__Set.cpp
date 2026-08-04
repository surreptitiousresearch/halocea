#include "../../headers/ws/txm/txmTEXTURE.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/pctPICTURE.h"
#include "../../headers/pct_boundary.h"

// 0x826F6488 -- ?Set@txmTEXTURE@@UAAHPAUpctPICTURE@@H@Z  (virtual)
// Assign a source picture to the texture. When isDelayed is set the picture is merely duplicated
// and stashed in pPictSrc with the "source pending" state bit 0x2000000, deferring the hardware
// upload. Otherwise the texture surface is (re)created to match the picture and the pixels are
// uploaded to the driver immediately (LoadDrv), clearing the pending bit and freeing pPictSrc.
// Returns 1 on success, 0 if surface creation or the driver upload fails.
int txmTEXTURE::Set(pctPICTURE *pPict, int isDelayed)
{
    if (!IGNORE_STRONG_ASSERT && this == (txmTEXTURE *)-20)
        STRONG_ASSERT_DUMMY().Crash("GetDesc()",
            "D:\\Projects\\code\\common\\src.sys\\drv\\video\\txm.cpp", 602, dsStrongAssertMessage);

    // state bit 0x400000: the surface already exists (has been Create()d).
    if (((this->state.state >> 22) & 1) == 0)
    {
        if (!isDelayed)
        {
            if (!this->Create(pPict->hdr.sx, pPict->hdr.sy, pPict->hdr.nMipMap,
                              pPict->hdr.format, pPict->hdr.nFaces, pPict->hdr.sz))
                return 0;
            goto upload;
        }

    stashSource:
        pctDestroy(this->pPictSrc);
        this->pPictSrc = pctDuplicate(pPict);
        // Set "source pending" (0x2000000) and clear the "source uploaded" group (0x2A00000).
        this->state.state |= 0x2000000u;
        this->state.state = (this->state.state & 0xFD5FFFFF) | 0x2000000u;
        return 1;
    }

    if (isDelayed)
        goto stashSource;

upload:
    if ((((this->state.state >> 22) & 1) == 0
         && !this->Create(pPict->hdr.sx, pPict->hdr.sy, pPict->hdr.nMipMap, pPict->hdr.format, 1, 1))
        || !this->LoadDrv(pPict))
    {
        return 0;
    }

    this->state.state &= ~0x2000000u;
    pctDestroy(this->pPictSrc);
    this->pPictSrc = nullptr;
    // Set "source uploaded" (0x200000) and clear the "source pending" group (0x1200000).
    this->state.state |= 0x200000u;
    this->state.state = (this->state.state & ~0x1200000u) | 0x200000u;
    return 1;
}
