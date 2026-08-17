/* ?TermHW@d3dDRIVER@@MAAXXZ @0x82691730 */
#include "headers/d3d_driver.h"
#include "headers/d3dDRIVER_INTERFACE.h" // com-buffer interfaces are d3dDRIVER_INTERFACE (pDevice@0x128)
#include "headers/vidCOMMAND_BUFFER_MANAGER.h" // typed vftable (dtr slot 0)

// --- boundary externs (deeper than this wave's descent) ---
struct vidLOCK { void Lock(const char *file, int line); void Unlock(const char *file, int line); };
extern vidLOCK vidLock;
extern int DEBUG_SceneCounter;
extern int DEBUG_WasBegin;
/* DEVIATION: D3DResource_Release was locally redeclared here as `void (void *)`, which under C++
 * overloaded the correct `unsigned int (D3DResource *)` that d3d_boundary.h (via d3d_driver.h)
 * already declares -- an unresolvable mangled reference at link. Local decl removed; the call
 * sites take the corpus-standard (D3DResource *) cast. */
extern "C" void D3DDevice_Release(D3DDevice *pDevice);
extern "C" void D3DDevice_SetRenderTarget_External(D3DDevice *pDevice, unsigned int index, void *pRT);
extern "C" void D3DDevice_SetDepthStencilSurface(D3DDevice *pDevice, void *pDepthStencil);
extern "C" void XPhysicalFree(void *p);
/* DEVIATION: ?__apMemoryDebugRemove@@YAHPAX_N@Z returns int and takes bool; the previous
 * `void (void *, int)` spelling mangled to a symbol the binary does not export. */
extern int __apMemoryDebugRemove(void *p, bool isAllocator);

// 0x82691730 -- d3dDRIVER::TermHW. Tears down the Xenon device. Clears the in-scene bit, resets
// the DEBUG scene counters, and (only if the device was actually up, bit 0x04000000) chains
// vidDRIVER::TermHW, then under the video lock: interface TermHW, release occlusion queries,
// unbind render/depth targets, drain the RT pool and front-buffer textures, tear down each
// command-buffer interface, destroy the command-buffer manager and page allocator, and release
// the depth/back buffers.
void d3dDRIVER::TermHW()
{
    this->base.state.state &= ~0x10000000u; // clear in-scene bit
    DEBUG_SceneCounter = 0;
    DEBUG_WasBegin = 0;
    if ( ((this->base.state.state >> 26) & 1) == 0 ) // device not ready -> nothing to tear down
        return;

    this->base.TermHW(); // base-subobject call (composition model) (2026-07-31)
    vidLock.Lock("D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\D3d_drv_8.cpp", 1346);

    this->base.drvInterface->TermHW();
    this->ReleaseQueries();
    D3DDevice_SetRenderTarget_External(this->pDevice, 0, nullptr);
    D3DDevice_SetDepthStencilSurface(this->pDevice, nullptr);
    this->XenonClearRTPool();

    // Release and null the two front-buffer textures. (Decompiler walked these via
    // &RTPool.__cl.line + 1/2, which alias m_pFrontBufferTexture[0]/[1].)
    for ( int i = 0; i < 2; ++i )
    {
        if ( this->m_pFrontBufferTexture[i] )
            D3DResource_Release((D3DResource *)this->m_pFrontBufferTexture[i]);
        this->m_pFrontBufferTexture[i] = nullptr;
    }

    // Tear down each command-buffer interface (IsValidIdx debug asserts omitted).
    dsCONST_ARRAY<vidDRIVER_INTERFACE *, 2> *pComBufIfaces = &this->base.drvComBufInterface;
    for ( int i = 0; i < pComBufIfaces->nEntry; ++i )
    {
        d3dDRIVER_INTERFACE *iface = (d3dDRIVER_INTERFACE *)pComBufIfaces->list[i];
        iface->AcquireThreadOwnership();
        // Typed vftable dispatch (vidDRIVER_INTERFACE_vtbl): slot 2 = TermHW, slot 0 = deleting dtor.
        iface->__vftable->TermHW(iface);
        D3DDevice_Release(iface->pDevice);
        if ( iface->__vftable )
            iface->__vftable->dtr_vidDRIVER_INTERFACE(iface, 1); // scalar-deleting destructor
        pComBufIfaces->list[i] = nullptr;
    }

    if ( this->base.comBufManager )
    {
        // Virtual scalar-deleting destructor via typed vftable slot 0.
        vidCOMMAND_BUFFER_MANAGER *mgr = this->base.comBufManager;
        mgr->__vftable->dtr_vidCOMMAND_BUFFER_MANAGER(mgr, 1);
    }
    this->base.comBufManager = nullptr;

    XPhysicalFree(this->pAllocator->allocatedMemory);
    __apMemoryDebugRemove(this->pAllocator->allocatedMemory, 0);
    vidPAGE_MEMORY_ALLOCATOR *pAllocator = this->pAllocator;
    if ( pAllocator )
    {
        pAllocator->labels.Realloc(0); // decompiler scope-qualified artifact -> member call (2026-07-31)
        pAllocator->labels.nElem = 0;
        operator delete(pAllocator);
    }
    this->pAllocator = nullptr;

    if ( this->pDepthBuffer )
        D3DResource_Release((D3DResource *)this->pDepthBuffer);
    this->pDepthBuffer = nullptr;
    if ( this->pBackBuffer )
        D3DResource_Release((D3DResource *)this->pBackBuffer);
    this->pBackBuffer = nullptr;

    vidLock.Unlock("D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\D3d_drv_8.cpp", 1346);
}
