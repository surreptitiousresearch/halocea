/* hcex_s3d_render_off @0x823CE2E8 */
/* HCEX render-mode selector (mangled: _hcex_new_render_mode__3HA). */
extern int hcex_new_render_mode;

/* Disable the stereoscopic-3D (s3d) render path. */
extern "C" void hcex_s3d_render_off(void)
{
    hcex_new_render_mode = 0;
}
