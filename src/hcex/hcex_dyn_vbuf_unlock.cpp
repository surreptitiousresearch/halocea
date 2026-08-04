/* hcex_dyn_vbuf_unlock @0x823EFE40 — finish writing vertices into the shared dynamic-geometry vertex stream.
 * Fetches the ws-engine dynamic-geometry hardware buffer (vidDriver->drvInterface->dynGeomBuffer), grabs the
 * current dynamic chunk (hcex_get_dyn_chunk), and dispatches WriteVerticesEnd(buffer, chunk) through its
 * vtable (slot 3). All vid* types are ws-engine boundaries. The nVert argument is unused by the binary
 * (kept for call-site symmetry). */

#include "../headers/vidDRIVER_DYNGEOM_BUFFER.h"

extern vidDRIVER *vidDriver;
extern vidDRIVER_DYNGEOM_CHUNK *hcex_get_dyn_chunk(void);

extern "C" void hcex_dyn_vbuf_unlock(unsigned int nVert)
{
    (void)nVert;
    vidDRIVER_DYNGEOM_BUFFER *dynGeomBuffer = vidDriver->drvInterface->dynGeomBuffer;
    vidDRIVER_DYNGEOM_CHUNK *dyn_chunk = hcex_get_dyn_chunk();
    dynGeomBuffer->__vftable->WriteVerticesEnd(dynGeomBuffer, dyn_chunk);
}
