/* hcex_is_halo_mode @ 0x823CE2B0
   True when the current render mode is the (legacy Blam) Halo mode (0). */

extern int hcex_cur_render_mode;

extern "C" int hcex_is_halo_mode(void)
{
    return hcex_cur_render_mode == 0;
}
