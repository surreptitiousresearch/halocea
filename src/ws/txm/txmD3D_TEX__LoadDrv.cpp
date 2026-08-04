// ?LoadDrv@txmD3D_TEX@@UAAHPAUpctPICTURE@@@Z  (0x827BC758)
// Upload a source picture's pixels into this hardware texture. Byte-reorders the source for
// endianness, logs the load (and any runtime format conversion), then locks each (face,mip)
// level of the driver surface and copies the converted pixels in. Handles cubemap, volume (3D)
// and 2D textures. Always returns 1.
// Source: D:\Projects\code\common\src.sys\drv\video\d3d_8\d3d_txm_8.cpp

#include "../../headers/ws/txm/txmD3D_TEX.h"
#include "../../headers/ws/txm/txm_d3d_loaddrv_boundary.h"
#include "../../headers/ws/ds/dsBIT_STREAM.h"    /* ds::ByteReorder */
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/pct_format.h"            /* PCT_FORMAT_DXN / PCT_FORMAT_DXT5A */

// txmTEXTURE::state bit 11 -- cubemap.
enum { TXM_STATE_CUBEMAP = 0x800 };

static const char *const D3D_TXM_CPP =
    "D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\d3d_txm_8.cpp";

// DEVIATION: the decompiler calls pctGetFormat(pPict) passing the picture pointer where the
// corrected DB signature is pctGetFormat(unsigned int formatCode) -- reconstructed as
// pctGetFormat(pPict->hdr.format) to match the real prototype (see pct_boundary.h note).

int txmD3D_TEX::LoadDrv(pctPICTURE *pPict)
{
    const char *kind;
    if (this->state.state & TXM_STATE_CUBEMAP)
        kind = "cubemap";
    else if (this->sizeZ <= 1)
        kind = "2d";
    else
        kind = "3d";

    _apLog("~Texture~Loading texture '%s' (%s %s SIZE : %d)(pct size %d) ",
           this->name.pBuffer->str, this->pFmt->name, kind, this->texSize,
           pctGetMemSize(&pPict->hdr));

    if (this->pFmt != pctGetFormat(pPict->hdr.format)) {
        _apLog("~Error,Texture~Runtime texture conversion! (%s from %s to %s)",
               this->name.pBuffer->str, pctGetFormat(pPict->hdr.format)->name, this->pFmt->name);
    }

    // Byte-reorder the source pixels for GPU endianness -- per-face for cubemaps, whole buffer
    // otherwise.
    if (this->state.state & TXM_STATE_CUBEMAP) {
        for (int face = 0; face < pPict->hdr.nFaces; ++face) {
            int bpp = pctGetFormat(pPict->hdr.format)->bpp;
            ds::ByteReorder((unsigned char *)pPict->pFaces[face],
                            pctGetMemFaceSize(&pPict->hdr), bpp);
        }
    } else {
        int bpp = pctGetFormat(pPict->hdr.format)->bpp;
        ds::ByteReorder((unsigned char *)pPict->pFaces[0], pctGetMemSize(&pPict->hdr), bpp);
    }

    if (this->state.state & TXM_STATE_CUBEMAP) {
        static const _D3DCUBEMAP_FACES faceOrder[6] = {
            D3DCUBEMAP_FACE_POSITIVE_X, D3DCUBEMAP_FACE_NEGATIVE_X,
            D3DCUBEMAP_FACE_POSITIVE_Y, D3DCUBEMAP_FACE_NEGATIVE_Y,
            D3DCUBEMAP_FACE_POSITIVE_Z, D3DCUBEMAP_FACE_NEGATIVE_Z,
        };
        for (int face = 0; face < pPict->hdr.nFaces; ++face) {
            for (int mip = 0; mip < this->nMipMap; ++mip) {
                _D3DLOCKED_RECT lr;
                vidLock.Lock(D3D_TXM_CPP, 1319);
                D3DCubeTexture_LockRect((D3DCubeTexture *)this->GetD3DTex(),
                                        faceOrder[face], mip, &lr, nullptr, 0);
                vidLock.Unlock(D3D_TXM_CPP, 1321);
                pctCopyBuf(pPict, face, mip, this->pFmt, lr.Pitch, (unsigned char *)lr.pBits, 0);
                vidLock.Lock(D3D_TXM_CPP, 1323);
                D3DCubeTexture_UnlockRect((D3DCubeTexture *)this->GetD3DTex(), faceOrder[face], mip);
                vidLock.Unlock(D3D_TXM_CPP, 1325);
            }
        }
        return 1;
    }

    if (this->sizeZ > 1) {
        // Volume textures: DXN/DXT5A block formats are unsupported for the volume path.
        if (!IGNORE_STRONG_ASSERT) {
            if (pPict->hdr.format != PCT_FORMAT_DXT5A)
                ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
                    "pPict->hdr.format!=PCT_FORMAT_DXT5A", D3D_TXM_CPP, 1331, dsStrongAssertMessage);
            if (pPict->hdr.format == PCT_FORMAT_DXN)
                ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
                    "pPict->hdr.format!=PCT_FORMAT_DXN", D3D_TXM_CPP, 1332, dsStrongAssertMessage);
        }
        for (int mip = 0; mip < this->nMipMap; ++mip) {
            _D3DLOCKED_BOX lb;
            vidLock.Lock(D3D_TXM_CPP, 1336);
            D3DVolumeTexture_LockBox((D3DVolumeTexture *)this->GetD3DTex(), mip, &lb, nullptr, 0);
            vidLock.Unlock(D3D_TXM_CPP, 1338);
            pctCopyBuf(pPict, 0, mip, this->pFmt, lb.RowPitch, (unsigned char *)lb.pBits, lb.SlicePitch);
            vidLock.Lock(D3D_TXM_CPP, 1340);
            D3DVolumeTexture_UnlockBox((D3DVolumeTexture *)this->GetD3DTex(), mip);
            vidLock.Unlock(D3D_TXM_CPP, 1342);
        }
        return 1;
    }

    // 2D textures.
    for (int mip = 0; mip < this->nMipMap; ++mip) {
        _D3DSURFACE_DESC sd;
        _D3DLOCKED_RECT lr;
        vidLock.Lock(D3D_TXM_CPP, 1346);
        D3DTexture_GetLevelDesc((D3DTexture *)this->GetD3DTex(), mip, &sd);
        D3DTexture_LockRect((D3DTexture *)this->GetD3DTex(), mip, &lr, nullptr, 0);
        vidLock.Unlock(D3D_TXM_CPP, 1357);
        pctCopyBuf(pPict, 0, mip, this->pFmt, lr.Pitch, (unsigned char *)lr.pBits, 0);
        vidLock.Lock(D3D_TXM_CPP, 1359);
        D3DTexture_UnlockRect((D3DTexture *)this->GetD3DTex(), mip);
        vidLock.Unlock(D3D_TXM_CPP, 1361);
    }
    return 1;
}
