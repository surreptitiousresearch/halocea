extern void rasterizer_decals_recreate_vbuf(void);
extern void *rasterizer_detail_objects_recreate_vbuf(void);

/* rasterizer_recreate_vbuf tail-returns rasterizer_detail_objects_recreate_vbuf's d3d vertex-buffer
 * pointer (disasm 0x836A5FF0); retyped void* to match the callee's true return and dropped the spurious
 * decompiler-invented uninitialized `decals_result` arg (the callee takes none). 2026-07-31 */
void *rasterizer_recreate_vbuf(void)
{
    rasterizer_decals_recreate_vbuf();
    return rasterizer_detail_objects_recreate_vbuf();
}
