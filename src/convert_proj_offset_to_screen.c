/* convert_proj_offset_to_screen @0x836AFB68 */
#include <stdint.h>

extern float hud_globals_get_scale(uint8_t in_multiplayer);

/* Converts a projection-space horizontal offset into screen pixels: scales by the
 * 640px reference width and divides by the current HUD scale. */
int convert_proj_offset_to_screen(float in_offset)
{
    return (int)((in_offset * 640.0f) / hud_globals_get_scale(0));
}
