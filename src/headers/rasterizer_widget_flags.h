#ifndef RASTERIZER_WIDGET_FLAGS_H
#define RASTERIZER_WIDGET_FLAGS_H

/* rasterizer_widget_flags — bit indices for the `flags` argument to _rasterizer_widget_begin.
 * Reconstruction (no DB enum) — derived from the D3D render-state calls the bits drive:
 *   bit 0 -> D3DDevice_SetRenderState_ZEnable       (depth test)
 *   bit 1 -> D3DDevice_SetRenderState_ZWriteEnable  (depth write)
 * Values are BIT INDICES — use as (flags >> _bit) & 1. */
enum rasterizer_widget_flags {
	_rasterizer_widget_z_enable_bit = 0,
	_rasterizer_widget_z_write_enable_bit = 1,
};

#endif /* RASTERIZER_WIDGET_FLAGS_H */
