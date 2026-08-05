extern void rasterizer_decals_recreate_vbuf(void);
extern void rasterizer_detail_objects_recreate_vbuf(void);

/* rasterizer_recreate_vbuf (disasm 0x836A5FF0) re-creates both vertex buffers; it dropped the spurious
 * decompiler-invented uninitialized `decals_result` arg (the callee takes none). 2026-07-31
 * DEVIATION: declared void — the r3 surviving the tail call is the callee's residue (that callee
 * publishes its buffer through local_d3d_vertex_buffer_0), and the sole caller ignores r3. */
void rasterizer_recreate_vbuf(void)
{
    rasterizer_decals_recreate_vbuf();
    rasterizer_detail_objects_recreate_vbuf();
}
