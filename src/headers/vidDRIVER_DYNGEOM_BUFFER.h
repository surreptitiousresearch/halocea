#pragma once
/* vidDRIVER_DYNGEOM_BUFFER — ws-engine dynamic-geometry hardware buffer (boundary type).
 * Reached from the global vidDriver via vidDriver->drvInterface->dynGeomBuffer. Only the vtable
 * and the reachability chain HCEX uses are modeled; the rest of the driver/interface state is
 * opaque. Chain offsets and vtable slots verified against disasm + types_members:
 *   vidDRIVER::drvInterface           @ 0x164
 *   vidDRIVER_INTERFACE::dynGeomBuffer@ 0x124
 *   vtbl slot WriteVerticesEnd        @ 0x0C (index 3)
 *   vtbl slot WriteIndicesEnd         @ 0x18 (index 6) */

#include "ws/ds/dsCONST_ARRAY.h"
#include "vidDRIVER_INTERFACE.h"  /* vidDRIVER_INTERFACE (split out; formerly inline here) */
#include "vidDRIVER.h"            /* vidDRIVER (home header; formerly inline here) */

// DB-verified layout (types_members vidDRIVER_DYNGEOM_CHUNK) — size 12. Full definition required:
// dsCONST_ARRAY<vidDRIVER_DYNGEOM_CHUNK,4> embeds it by value.
typedef struct vidDRIVER_DYNGEOM_CHUNK {
    bool          used;     // 0x00
    unsigned int  callerID; // 0x04
    void         *data;     // 0x08
} vidDRIVER_DYNGEOM_CHUNK;
typedef struct vidDRIVER_DYNGEOM_BUFFER  vidDRIVER_DYNGEOM_BUFFER;
typedef struct vidFVF_DESCR              vidFVF_DESCR;

/* nested unnamed enum vidDRIVER_DYNGEOM_BUFFER::<unnamed_tag> : __int32 */
enum { CHUNKS_COUNT = 4 };

typedef struct vidDRIVER_DYNGEOM_BUFFER_vtbl
{
    void (*dtr)(vidDRIVER_DYNGEOM_BUFFER *self);                                         /* 0x00 */
    vidDRIVER_DYNGEOM_CHUNK *(*Begin)(vidDRIVER_DYNGEOM_BUFFER *self, unsigned int);     /* 0x04 */
    bool (*WriteVerticesBegin)(vidDRIVER_DYNGEOM_BUFFER *self, vidDRIVER_DYNGEOM_CHUNK *, unsigned int, int, unsigned int *, void **); /* 0x08 */
    void (*WriteVerticesEnd)(vidDRIVER_DYNGEOM_BUFFER *self, vidDRIVER_DYNGEOM_CHUNK *);  /* 0x0C */
    bool (*WriteVertices)(vidDRIVER_DYNGEOM_BUFFER *self, vidDRIVER_DYNGEOM_CHUNK *, const void *, unsigned int, int, unsigned int *); /* 0x10 */
    bool (*WriteIndicesBegin)(vidDRIVER_DYNGEOM_BUFFER *self, vidDRIVER_DYNGEOM_CHUNK *, int, unsigned int *, unsigned short **); /* 0x14 */
    void (*WriteIndicesEnd)(vidDRIVER_DYNGEOM_BUFFER *self, vidDRIVER_DYNGEOM_CHUNK *);   /* 0x18 */
    bool (*WriteIndices)(vidDRIVER_DYNGEOM_BUFFER *self, vidDRIVER_DYNGEOM_CHUNK *, const void *, int, unsigned int *); /* 0x1C */
    void (*Draw)(vidDRIVER_DYNGEOM_BUFFER *self, vidFVF_DESCR *, int, unsigned int, int, unsigned int, int); /* 0x20 */
    void (*End)(vidDRIVER_DYNGEOM_BUFFER *self, vidDRIVER_DYNGEOM_CHUNK *);              /* 0x24 */
    void (*Flush)(vidDRIVER_DYNGEOM_BUFFER *self);                                       /* 0x28 */
} vidDRIVER_DYNGEOM_BUFFER_vtbl;

struct vidDRIVER_DYNGEOM_BUFFER
{
    vidDRIVER_DYNGEOM_BUFFER_vtbl *__vftable;            /* 0x00 */
    dsCONST_ARRAY<vidDRIVER_DYNGEOM_CHUNK, 4> chunks;    /* 0x04 */
    vidDRIVER_INTERFACE *device;                         /* 0x38 */
    void *vertexBuffer;                                  /* 0x3C */
}; /* 0x40 bytes */
