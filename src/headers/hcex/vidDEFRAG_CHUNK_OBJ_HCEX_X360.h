#pragma once
/* vidDEFRAG_CHUNK_OBJ_HCEX_X360 — ws-engine defrag-managed GPU buffer chunk (Xbox 360), the HCEX
 * static vertex/index buffer object.
 *
 * This path used to carry a second, flat POD twin of vidBUF_CONTENT / vidDEFRAG_CHUNK /
 * vidDEFRAG_CHUNK_OBJ_HCEX_X360 for "the legacy .c consumers". There are none: every TU that
 * reaches this header is a .cpp in the C++ corpus (hcexRenderInit, hcex_stat_ibuf_create,
 * hcex_stat_vbuf_create, hcex_stat_vbuf_create_semi_dyn, apDEFRAG_MNG__Alloc), so the twin bought
 * nothing and cost three duplicate definitions. It also modelled the apDEFRAG_CHUNK /
 * vidDEFRAG_CHUNK bases as named `base` members, whereas types_members marks both as
 * is_baseclass=1 at offset 0 — the C++ class below is the layout the DB actually describes.
 *
 * Kept as a forwarding header because the include path is referenced by name from five TUs. */

#include "../ws/vid/vidDEFRAG_CHUNK_OBJ_HCEX_X360.h"
