/* hcex_s3d_render_on @ 0x823CE2C8
   Enables the stereoscopic-3D (s3d) render path and suppresses the normal
   drawing pass. hcex_new_render_mode is the HCEX render-mode selector (mangled
   _hcex_new_render_mode__3HA); debug_no_drawing is a Blam engine debug flag. */
#include <stdint.h>

extern int hcex_new_render_mode;
extern uint8_t debug_no_drawing;

extern "C" void hcex_s3d_render_on(void)
{
    hcex_new_render_mode = 1;
    debug_no_drawing = 1;
}
