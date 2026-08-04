/* hcex_dyn_ibuf_lock @0x823EFCD8 — begin writing indices into the shared dynamic-geometry index stream.
 * Fetches the ws-engine dynamic-geometry hardware buffer (vidDriver->drvInterface->dynGeomBuffer) and the
 * current dynamic chunk (hcex_get_dyn_chunk), then dispatches WriteIndicesBegin(buffer, chunk, 3*nFace, ...)
 * through the vtable (slot 5) to reserve 3 indices per face and hand back the write pointer via buf_ptr. The
 * base index of the reserved range is returned. All vid* types are ws-engine boundaries. */

#include "../headers/vidDRIVER_DYNGEOM_BUFFER.h"

extern vidDRIVER *vidDriver;
extern vidDRIVER_DYNGEOM_CHUNK *hcex_get_dyn_chunk(void);

extern "C" int hcex_dyn_ibuf_lock(int nFace, unsigned short **buf_ptr)
{
    unsigned int base_index = 0;
    vidDRIVER_DYNGEOM_BUFFER *dynGeomBuffer = vidDriver->drvInterface->dynGeomBuffer;
    vidDRIVER_DYNGEOM_BUFFER_vtbl *vtbl = dynGeomBuffer->__vftable;
    vidDRIVER_DYNGEOM_CHUNK *dyn_chunk = hcex_get_dyn_chunk();

    vtbl->WriteIndicesBegin(dynGeomBuffer, dyn_chunk, 3 * nFace, &base_index, buf_ptr);
    return base_index;
}
