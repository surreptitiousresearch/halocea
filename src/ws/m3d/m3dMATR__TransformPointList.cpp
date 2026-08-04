#include "m3dMATR.h"

// Xbox 360 XNAMath intrinsics — external boundary. XMMATRIX is given a minimal complete
// stand-in (its real layout is 4 XMVECTORs = 64 bytes) so the by-value load type-checks.
struct XMFLOAT3;
struct XMFLOAT4X4;
struct XMMATRIX { float _r[16]; };
extern "C" XMMATRIX XMLoadFloat4x4(const XMFLOAT4X4 *pSource);
extern "C" void XMVector3TransformCoordStream(
    XMFLOAT3 *pOutputStream, unsigned int OutputStride,
    const XMFLOAT3 *pInputStream, unsigned int InputStride,
    unsigned int VectorCount, const XMMATRIX *pM);

// m3dMATR::TransformPointList @ 0x82544A68
// Transform a strided array of `nPoint` 3-component points by this matrix (as coordinates,
// i.e. with translation) into the destination stream.
// DEVIATION: the decompiler rendered the matrix load as raw VMX unaligned loads
// (lvlx128/lvrx128 + stvx128 into a stack XMMATRIX); that is exactly XMLoadFloat4x4 over
// the unaligned 4x4 element block, reconstructed here as that intrinsic.
void m3dMATR::TransformPointList(int nPoint, void *srcPtr, int srcStride,
                                 void *dstPtr, int dstStride) const
{
    XMMATRIX transform = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4 *>(elements));
    XMVector3TransformCoordStream(
        reinterpret_cast<XMFLOAT3 *>(dstPtr), dstStride,
        reinterpret_cast<const XMFLOAT3 *>(srcPtr), srcStride,
        nPoint, &transform);
}
