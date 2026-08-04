/* _rasterizer_environment_transparent_geometry_begin @0x837A8390 — thunk: begin the transparent geometry
 * group accumulation. */

extern void rasterizer_transparent_geometry_groups_begin(void);

void _rasterizer_environment_transparent_geometry_begin(void)
{
    rasterizer_transparent_geometry_groups_begin();
}
