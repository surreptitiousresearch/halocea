/* hcex_change_screen_mode @0x836EAF38 — hcex bridge: switch the current screen/render mode. When the
 * requested mode differs from curMode it is stored, then the ws-side stereo-3D renderer is toggled
 * (mode!=0 turns the s3d render path OFF, mode==0 turns it ON) and the input window state is refreshed.
 *
 * DEVIATION: the raw decompile threaded a return value from hcex_s3d_render_off/on into
 * hcex_input_update_window (`v1 = hcex_s3d_render_off(); hcex_input_update_window(v1);`). Both s3d toggles
 * are `void` (they only set hcex_new_render_mode) and hcex_input_update_window takes no argument — the
 * threaded r3 is a decompiler artifact, dropped here. */

extern int curMode;

extern void hcex_s3d_render_off(void);
extern void hcex_s3d_render_on(void);
extern void hcex_input_update_window(void);

extern "C" void hcex_change_screen_mode(int mode)
{
    if ( mode != curMode )
    {
        curMode = mode;
        if ( mode )
            hcex_s3d_render_off();
        else
            hcex_s3d_render_on();
        hcex_input_update_window();
    }
}
