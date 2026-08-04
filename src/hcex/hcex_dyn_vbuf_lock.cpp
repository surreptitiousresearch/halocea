/* hcex_dyn_vbuf_lock @0x823EFDD8 — begin writing vertices into the shared dynamic-geometry vertex stream.
 * Fetches the ws-engine dynamic-geometry hardware buffer (vidDriver->drvInterface->dynGeomBuffer) and the
 * current dynamic chunk (hcex_get_dyn_chunk), then dispatches WriteVerticesBegin(buffer, chunk, vertexSize,
 * nVert, &base_index, buf_ptr) through the vtable (slot 2) to reserve nVert vertices and hand back the write
 * pointer via buf_ptr. The base index of the reserved range is returned. All vid* types are ws-engine
 * boundaries. Mirror of hcex_dyn_ibuf_lock / hcex_dyn_vbuf_unlock. */

#include "../headers/vidDRIVER_DYNGEOM_BUFFER.h"

extern vidDRIVER *vidDriver;
extern vidDRIVER_DYNGEOM_CHUNK *hcex_get_dyn_chunk(void);

extern "C" int hcex_dyn_vbuf_lock(int nVert, unsigned int vertexSize, void **buf_ptr)
{
    unsigned int base_index = 0;
    vidDRIVER_DYNGEOM_BUFFER *dynGeomBuffer = vidDriver->drvInterface->dynGeomBuffer;
    vidDRIVER_DYNGEOM_BUFFER_vtbl *vtbl = dynGeomBuffer->__vftable;
    vidDRIVER_DYNGEOM_CHUNK *dyn_chunk = hcex_get_dyn_chunk();

    vtbl->WriteVerticesBegin(dynGeomBuffer, dyn_chunk, vertexSize, nVert, &base_index, buf_ptr);
    return base_index;
}
