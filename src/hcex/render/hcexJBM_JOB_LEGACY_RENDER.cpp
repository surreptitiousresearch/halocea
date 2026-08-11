// hcexJBM_JOB_LEGACY_RENDER -- job-manager job wrapping the legacy Blam renderer. See
// src/headers/hcex/hcexJBM_JOB_LEGACY_RENDER.h for the DB-verified layout.

#include "../../headers/hcex/hcexJBM_JOB_LEGACY_RENDER.h"
#include "../../headers/ws/vid/vidLOCK.h"

void operator delete(void *ptr); // boundary -- CRT operator delete

extern vidLOCK vidLock;

// @0x823F08C8
hcexJBM_JOB_LEGACY_RENDER::hcexJBM_JOB_LEGACY_RENDER()
{
    this->name = "legacy_render";
    this->__vftable = (jbmJOB_vtbl *)&hcexJBM_JOB_LEGACY_RENDER::vftable;
    this->state.state = 0;
}

// 0x823EF6B8 -- thunk: re-point the vtable back to the jbmJOB base before base teardown.
hcexJBM_JOB_LEGACY_RENDER::~hcexJBM_JOB_LEGACY_RENDER()
{
    this->__vftable = (jbmJOB_vtbl *)&jbmJOB::vftable;
}

// 0x823F090C
hcexJBM_JOB_LEGACY_RENDER *hcexJBM_JOB_LEGACY_RENDER::ScalarDeletingDtor(unsigned char deleteFlags)
{
    this->__vftable = (jbmJOB_vtbl *)&jbmJOB::vftable;
    if (deleteFlags & 1)
    {
        operator delete(this);
        return this;
    }
    return this;
}

// 0x823EF6F8
void hcexJBM_JOB_LEGACY_RENDER::Execute(int threadID)
{
    // DEVIATION: the decompiler showed both Lock/Unlock calls with (nullptr, 0) file/line
    // arguments -- disasm confirms this is the real call, not a dropped argument (li r4,0 / li
    // r5,0 are the literal file/line operands here, unlike ResetHW's calls elsewhere which pass
    // real __FILE__/__LINE__ strings).
    vidLock.Lock(nullptr, 0);

    hcex_legacy_render();
    raserizer_release_fx_textures(); // HRESULT result discarded, matching the disassembly

    if (hcex_dyn_chunk)
    {
        // DEVIATION: the decompiler cast this vtable call to a 1-arg function pointer, but the
        // disasm shows r4 still holds `hcex_dyn_chunk` (loaded a few instructions earlier for the
        // null check and never overwritten) at the `bctrl` -- the real call is the 2-arg
        // End(self, chunk) slot from vidDRIVER_DYNGEOM_BUFFER_vtbl.
        vidDRIVER_DYNGEOM_BUFFER *dynGeomBuffer = vidDriver->drvInterface->dynGeomBuffer;
        dynGeomBuffer->__vftable->End(dynGeomBuffer, hcex_dyn_chunk);
        hcex_dyn_chunk = nullptr;
    }

    // __vftable is typed as the vidMEM_MNG_vtbl base (opaque); DiscardDynAll lives in the derived
    // vidVBUF_MNG_vtbl (index 12) -- cast to reach it.
    reinterpret_cast<vidVBUF_MNG_vtbl *>(vidVBMng->__vftable)->DiscardDynAll(vidVBMng);

    vidLock.Unlock(nullptr, 0);
}
