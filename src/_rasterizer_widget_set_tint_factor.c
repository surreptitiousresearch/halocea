/* _rasterizer_widget_set_tint_factor @0x83786110 */
#include <stdint.h>
#include "headers/blam_data_globals.h"
/* _rasterizer_widget_set_tint_factor 0x83786110 — set the widget specular tint to a uniform grey whose level is
 * the tint factor scaled to a byte (tint_factor * 255), replicated across all four ARGB channels. */


void _rasterizer_widget_set_tint_factor(float tint_factor)
{
    /* DEVIATION: the decompiler read the byte extraction as ">> 24"; the disassembly is clrlwi (& 0xFF). */
    unsigned int channel = (unsigned int)(int64_t)(tint_factor * 255.0f) & 0xFF;
    widget_specular_color = (((((channel << 8) | channel) << 8) | channel) << 8) | channel;
}
