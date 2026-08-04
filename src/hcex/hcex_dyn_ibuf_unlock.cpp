/* hcex_dyn_ibuf_unlock @0x823EFD40 — finish writing indices into the shared dynamic-geometry index stream.
 * Fetches the ws-engine dynamic-geometry hardware buffer (vidDriver->drvInterface->dynGeomBuffer), grabs the
 * current dynamic chunk (hcex_get_dyn_chunk), and dispatches WriteIndicesEnd(buffer, chunk) through its
 * vtable (slot 6). All vid* types are ws-engine boundaries. The nFace argument is unused by the binary
 * (kept for call-site symmetry). */

#include "../headers/vidDRIVER_DYNGEOM_BUFFER.h"

extern vidDRIVER *vidDriver;
extern vidDRIVER_DYNGEOM_CHUNK *hcex_get_dyn_chunk(void);

extern "C" void hcex_dyn_ibuf_unlock(int nFace)
{
    (void)nFace;
    vidDRIVER_DYNGEOM_BUFFER *dynGeomBuffer = vidDriver->drvInterface->dynGeomBuffer;
    vidDRIVER_DYNGEOM_CHUNK *dyn_chunk = hcex_get_dyn_chunk();
    dynGeomBuffer->__vftable->WriteIndicesEnd(dynGeomBuffer, dyn_chunk);
}
