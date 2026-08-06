#pragma once
/* dynamic_triangles @ dynamic_triangles — the per-frame dynamic index-buffer pool. Only the counts
 * are reset at begin-time. */

#include "dynamic_triangle_buffer.h"

typedef struct D3DIndexBuffer D3DIndexBuffer;

/* DB anonymous struct: types_members _A11AF50D3B09780998A5B6AB25D9CABC */
typedef struct dynamic_triangles_struct
{
    dynamic_triangle_buffer buffers[1024]; /* 0x0000 */
    int             buffer_count;       /* 0x3000 */
    int             triangle_count;     /* 0x3004 */
    D3DIndexBuffer *d3d_index_buffer;   /* 0x3008 */
} dynamic_triangles_struct;             /* 0x300C */

#ifdef __cplusplus
extern "C" {
#endif

extern dynamic_triangles_struct dynamic_triangles;

#ifdef __cplusplus
}
#endif
