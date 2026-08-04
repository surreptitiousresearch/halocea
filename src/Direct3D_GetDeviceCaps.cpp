/* Direct3D_GetDeviceCaps @0x8243D958 — Saber's thin wrapper over the D3D "get device caps"
 * query. On the Xbox 360 there is only one real device type (D3DDEVTYPE_HAL) and the caps are
 * fixed, so instead of round-tripping into the D3D SDK this returns a snapshot copied from the
 * cached D3D::g_DeviceCaps blob (populated once at device creation). Any non-HAL request is
 * rejected with the same failure HRESULT the SDK would return. Adapter is ignored (single
 * adapter). Actual D3D SDK types stay a boundary. */

#include "headers/_D3DCAPS9.h"
#include "headers/_D3DDEVTYPE.h"

typedef long HRESULT;

/* Cached device caps, filled at device-init time (D3D SDK boundary global). */
namespace D3D { extern _D3DCAPS9 g_DeviceCaps; }

extern "C" void *memcpy(void *dst, const void *src, unsigned int n);

HRESULT Direct3D_GetDeviceCaps(unsigned int Adapter, _D3DDEVTYPE DeviceType, _D3DCAPS9 *pCaps)
{
    (void)Adapter; // single adapter on X360 — ignored
    if (DeviceType != D3DDEVTYPE_HAL)
        return (HRESULT)-2005530517; // 0x8876086B — SDK "invalid device type" failure code

    memcpy(pCaps, &D3D::g_DeviceCaps, sizeof(_D3DCAPS9));
    return 0;
}
