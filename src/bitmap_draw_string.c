/* bitmap_draw_string @0x8376B528 — software (CPU) path for drawing an ASCII string into a bitmap.
 * Only a fixed set of uncompressed pixel formats can be drawn to in software; for those it stashes the
 * destination bitmap in draw_character_software_globals, clamps the caller's bounds and clip rectangles to
 * the bitmap's extent, and forwards to the generic draw_string glyph walker with bitmap_draw_character as
 * the per-character emitter. Unsupported formats draw nothing.
 *
 * DEVIATION: the `bounds == NULL` branch reads a default rectangle2d from absolute address 0 (the
 * decompiler's MEMORY[0/2/4/6]; disasm confirms `lhz rD, {0,2,4,6}` with base register rA=0). This is a
 * provably-dead defensive default — every caller passes a non-null bounds, and a real read of VA 0 is
 * outside the image and would fault — reproduced verbatim for fidelity via a rectangle2d* at null.
 * DEVIATION: `__CFADD__(x, 0x80000000)` on a sign-extended short is the carry idiom for `(short)x < 0`,
 * so `__CFADD__(x, 0x80000000) ? 0 : x` == max(0, (short)x); reproduced as an explicit clamp. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_format.h"
#include "headers/rectangle2d.h"
#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/point2d.h"
#include "headers/draw_character_software_globals.h"

#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/point2d.h"
#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
extern void set_rectangle2d(rectangle2d *rectangle, int16_t x0, int16_t y0, int16_t x1, int16_t y1);
extern void bitmap_draw_character(parse_string_state *parse_state, font_header *font_header, font_character *character, unsigned int color, int16_t x0, int16_t y0, int16_t x, int16_t y, int16_t dx, int16_t dy);
extern void draw_string(void (*draw_character)(parse_string_state *, font_header *, font_character *, unsigned int, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t), const rectangle2d *bounds, point2d *cursor_reference, const rectangle2d *clip, int16_t height_adjust, const char *string);

void bitmap_draw_string(bitmap_data *bitmap, const rectangle2d *bounds, const rectangle2d *clip,
        const char *string)
{
    const rectangle2d *effective_bounds = bounds;
    unsigned __int16 format = (unsigned __int16)bitmap->format;

    draw_character_software_globals.bitmap = bitmap;

    unsigned char format_supported;
    switch ( format )
    {
        case _bitmap_format_a8:
        case _bitmap_format_y8:
        case _bitmap_format_ay8:
        case _bitmap_format_r5g6b5:
        case _bitmap_format_a8r8g8b8:
            format_supported = 1;
            break;
        default:
            format_supported = 0;
            break;
    }
    if ( !format_supported )
        return;

    rectangle2d clamped_bounds;
    rectangle2d clamped_clip;

    if ( !bounds )
    {
        const rectangle2d *default_bounds = (const rectangle2d *)0;   /* dead null-bounds default (see header) */

        __int16 bottom = default_bounds->y1;
        if ( bitmap->height <= default_bounds->y1 )
            bottom = bitmap->height;
        __int16 right = default_bounds->x1;
        if ( bitmap->width <= default_bounds->x1 )
            right = bitmap->width;

        __int16 left = default_bounds->x0 < 0 ? 0 : default_bounds->x0;
        __int16 top = default_bounds->y0 < 0 ? 0 : default_bounds->y0;

        set_rectangle2d(&clamped_bounds, left, top, right, bottom);
        effective_bounds = &clamped_bounds;
    }

    if ( clip )
    {
        __int16 bottom = clip->y1;
        if ( bitmap->height <= clip->y1 )
            bottom = bitmap->height;
        __int16 right = clip->x1;
        if ( bitmap->width <= clip->x1 )
            right = bitmap->width;

        set_rectangle2d(&clamped_clip,
                clip->x0 < 0 ? 0 : clip->x0,
                clip->y0 < 0 ? 0 : clip->y0,
                right, bottom);
        clip = &clamped_clip;
    }

    draw_string(bitmap_draw_character, effective_bounds, nullptr, clip, 0, string);
}
