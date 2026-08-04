#pragma once
/* font_drawing_globals — current text-drawing state (font, color, style, justification, tab stops, indents).
 * Layout from the database; the trailing multitexture_params block is a
 * rasterizer_dynamic_screen_geometry_parameters (140 bytes). */

#include "real_argb_color.h"
#include "rasterizer_dynamic_screen_geometry_parameters.h"

struct font_drawing_globals
{
    int             string_list_index;     /* 0x00 */
    int             current_font_index;     /* 0x04 */
    unsigned int    current_flags;          /* 0x08 */
    __int16         current_style;          /* 0x0C */
    __int16         current_justification;  /* 0x0E */
    real_argb_color current_color;          /* 0x10 */
    __int16         tab_stop_count;         /* 0x20 */
    __int16         tab_stops[16];          /* 0x22 */
    __int16         highlight_start_index;  /* 0x42 */
    __int16         highlight_stop_index;   /* 0x44 */
    __int16         initial_indent;         /* 0x46 */
    __int16         paragraph_indent;       /* 0x48 */
    unsigned char _pad0[2]; /* db-verified padding */
    /* 0x4A: 2 pad bytes (struct alignment) */
    rasterizer_dynamic_screen_geometry_parameters multitexture_params; /* 0x4C */
};                     /* 216 bytes */

extern struct font_drawing_globals font_drawing_globals;
