/* rasterizer_dynamic_triangles_new 0x836A5DD0 (thunk) — tail-call thunk to the underscore implementation. */
extern int _rasterizer_dynamic_triangles_new(int count);

int rasterizer_dynamic_triangles_new(int count)
{
    return _rasterizer_dynamic_triangles_new(count);
}
