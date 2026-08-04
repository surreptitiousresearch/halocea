/* _rasterizer_dynamic_triangles_lock 0x83791388 — lock one dynamic-triangle sub-buffer's index range for CPU
 * writing. Asks the hcex index-buffer backend for a write pointer (stored back into the buffer's data field) and
 * records the returned index offset (in triangles, hence >> 1) as the buffer's start offset, then caches the live
 * D3D index buffer. Returns the buffer's triangle write pointer, or null for an invalid index. */

#include "headers/dynamic_triangles.h"
#include "headers/rasterizer_triangle.h"

extern dynamic_triangles_struct dynamic_triangles;

extern unsigned int hcex_dyn_ibuf_lock(int nFace, unsigned __int16 **buf_ptr);
extern D3DIndexBuffer *hcex_dyn_ibuf_get();

rasterizer_triangle *_rasterizer_dynamic_triangles_lock(int dynamic_triangle_buffer_index)
{
    if ( dynamic_triangle_buffer_index == -1 )
        return nullptr;

    dynamic_triangle_buffer *buffer = &dynamic_triangles.buffers[dynamic_triangle_buffer_index];
    buffer->triangle_start_ofs = hcex_dyn_ibuf_lock(buffer->triangle_count,
                                                    (unsigned __int16 **)&buffer->data) >> 1;
    dynamic_triangles.d3d_index_buffer = hcex_dyn_ibuf_get();
    return buffer->data;
}
