#pragma once
/* font_drawing_globals — current text-drawing state (font, color, style, justification, tab stops, indents).
 * Layout from the database; the trailing multitexture_params block is a
 * rasterizer_dynamic_screen_geometry_parameters (140 bytes). */

#include <stdint.h>
#include "real_argb_color.h"
#include "rasterizer_dynamic_screen_geometry_parameters.h"

struct font_drawing_globals
{
    int             string_list_index;     /* 0x00 */
    int             current_font_index;     /* 0x04 */
    unsigned int    current_flags;          /* 0x08 */
    int16_t         current_style;          /* 0x0C */
    int16_t         current_justification;  /* 0x0E */
    real_argb_color current_color;          /* 0x10 */
    int16_t         tab_stop_count;         /* 0x20 */
    int16_t         tab_stops[16];          /* 0x22 */
    int16_t         highlight_start_index;  /* 0x42 */
    int16_t         highlight_stop_index;   /* 0x44 */
    int16_t         initial_indent;         /* 0x46 */
    int16_t         paragraph_indent;       /* 0x48 */
    unsigned char _pad0[2]; /* db-verified padding */
    /* 0x4A: 2 pad bytes (struct alignment) */
    rasterizer_dynamic_screen_geometry_parameters multitexture_params; /* 0x4C */
};                     /* 216 bytes */

extern struct font_drawing_globals font_drawing_globals;
