/* _rasterizer_environment_transparent_geometry_end @0x837A8640 — thunk: flush the accumulated transparent
 * geometry groups. */

extern void rasterizer_transparent_geometry_groups_end(void);

void _rasterizer_environment_transparent_geometry_end(void)
{
    rasterizer_transparent_geometry_groups_end();
}
