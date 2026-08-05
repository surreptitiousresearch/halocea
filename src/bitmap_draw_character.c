/* bitmap_draw_character @0x83769C40 — software (CPU) glyph blitter: alpha-blends one font glyph's
 * coverage bitmap into the destination bitmap stashed in draw_character_software_globals. It reads
 * dx x dy source coverage pixels starting at (x, y) in the glyph atlas and blends them into the
 * destination starting at (x0, y0), with the blend arithmetic chosen by the destination bitmap's
 * pixel format: 8-bit alpha (a8/y8/ay8), R5G6B5 (r5g6b5), or A8R8G8B8 (a8r8g8b8). The requested
 * colour's alpha byte (color>>24) scales each source coverage sample. parse_state is unused.
 *
 * Reconstructed from disassembly (0x83769C40-0x83769F88). The decompiler fabricated a 30-argument
 * phantom pile from mis-sized stack slots; the DB prototype (and the two real stack loads at arg_56 /
 * arg_5E) confirm the true 10-arg signature — dx (arg 9) is the per-row pixel width / inner-loop count
 * and dy (arg 10) is the glyph row count / outer-loop count. For the A8R8G8B8 path the decompiler's
 * BYTE1/BYTE2 channel labels are swapped; the real extracts are R=(p>>16)&0xFF, G=(p>>8)&0xFF,
 * B=p&0xFF, A=p>>24 (decoded from the extrwi operands). */

#include <stdint.h>
#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/bitmap_data.h"
#include "headers/bitmap_format.h"
#include "headers/draw_character_software_globals.h"

extern int16_t bitmap_format_get_bits_per_pixel(int16_t format);

void bitmap_draw_character(parse_string_state *parse_state, font_header *font_header, font_character *character, unsigned int color, int16_t x0, int16_t y0, int16_t x, int16_t y, int16_t dx, int16_t dy)
{
    (void)parse_state;

    bitmap_data *bitmap = draw_character_software_globals.bitmap;
    unsigned int coverage_scale = color >> 24;
    char *glyph_pixels = (char *)font_header->pixels.address + character->pixels_offset;
    int format = bitmap->format;

    uint16_t destination_color_16;
    /* ARGB8888 -> R5G6B5 channel pack: 0xFF00/0xFC/>>3 are pixel-format channel extracts, not flag masks. */
    if ( format == _bitmap_format_r5g6b5 )
        destination_color_16 = (8 * ((color >> 11) & 0xFF00 | (color >> 8) & 0xFC))
                | ((uint8_t)color >> 3);
    else
        destination_color_16 = 0; /* dead: only read on the r5g6b5 path (was uninitialized stack) */

    if ( dy <= 0 )
        return;

    for ( int row = dy; row > 0; --row, ++y, ++y0 )
    {
        int16_t bits_per_pixel = bitmap_format_get_bits_per_pixel(format);
        char *source_pixel = &glyph_pixels[character->bitmap_width * y + x];
        unsigned char *destination_pixel = (unsigned char *)bitmap->base_address
                + (x0 << draw_character_software_globals.encoding_shift)
                + y0 * (bits_per_pixel * bitmap->width / 8);

        switch ( format )
        {
            case _bitmap_format_a8:
            case _bitmap_format_y8:
            case _bitmap_format_ay8:
                for ( int column = dx; column > 0; --column, ++source_pixel, ++destination_pixel )
                {
                    if ( *source_pixel )
                    {
                        unsigned char destination_value = *destination_pixel;
                        int16_t coverage = (int16_t)(((int16_t)coverage_scale
                                * (uint8_t)*source_pixel) >> 8);
                        if ( coverage <= destination_value )
                            destination_value = (unsigned char)coverage;
                        *destination_pixel = destination_value;
                    }
                }
                break;

            case _bitmap_format_r5g6b5:
                for ( int column = dx; column > 0; --column, ++source_pixel, destination_pixel += 2 )
                {
                    if ( *source_pixel )
                    {
                        int16_t coverage = (int16_t)(((int16_t)coverage_scale
                                * (uint8_t)*source_pixel) >> 8);
                        int16_t inverse = (int16_t)(255 - coverage);
                        uint16_t destination = *(uint16_t *)destination_pixel;
                        *(uint16_t *)destination_pixel =
                              ((destination * inverse + destination_color_16 * coverage) >> 8) & 0xF800
                            | (((destination & 0x1F) * inverse + (destination_color_16 & 0x1F) * coverage) >> 8) & 0x1F
                            | (((destination & 0x7FF) * inverse + (destination_color_16 & 0x7FF) * coverage) >> 8) & 0x7E0;
                    }
                }
                break;

            case _bitmap_format_a8r8g8b8:
                for ( int column = dx; column > 0; --column, ++source_pixel, destination_pixel += 4 )
                {
                    if ( *source_pixel )
                    {
                        unsigned int destination = *(unsigned int *)destination_pixel;
                        int16_t coverage = (int16_t)(((int16_t)coverage_scale
                                * (uint8_t)*source_pixel) >> 8);
                        int16_t inverse = (int16_t)(255 - coverage);
                        unsigned int destination_alpha = destination >> 24;
                        if ( coverage > destination_alpha )
                            destination_alpha = coverage;

                        unsigned int red = (((destination >> 16) & 0xFF) * inverse & 0xFFFFFF00)
                                + (((color >> 16) & 0xFF) * coverage & 0xFFFFFF00);
                        unsigned int green = (((destination >> 8) & 0xFF) * inverse & 0xFFFFFF00)
                                + (((color >> 8) & 0xFF) * coverage & 0xFFFFFF00);
                        unsigned int blue = (((destination & 0xFF) * inverse) >> 8)
                                + (((color & 0xFF) * coverage) >> 8);
                        *(unsigned int *)destination_pixel =
                                (red << 8) | green | blue | (destination_alpha << 24);
                    }
                }
                break;

            default:
                break;
        }
    }
}
