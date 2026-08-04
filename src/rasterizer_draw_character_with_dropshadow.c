/* rasterizer_draw_character_with_dropshadow @0x83769180 — draw one glyph as a textured screen quad with a
 * one-pixel drop shadow. The glyph is first cached into the hardware character bitmap; if it has no cache
 * slot nothing is drawn. Otherwise the quad is emitted twice: a shadow pass offset by (+1,+1) in the shadow
 * colour (the global override, or the alpha channel of the text colour), then the glyph itself at its true
 * position in the text colour. The screen rectangle is (x0,y0)..(x0+dx,y0+dy) and the matching glyph-cache
 * texels run (u,v)..(u+dx,v+dy), where (u,v) is the cache slot origin offset by (x,y).
 *
 * DEVIATION: the decompiler failed local-variable allocation here ("output may be wrong") and rendered the
 * int->float corner conversions as __int128/OVERLAPPED dword shuffles with phantom parameters a11..a30.
 * Reconstructed from the disassembly: there are 10 real parameters (the DB prototype), the quad vertices
 * are TL,TR,BR,BL, and the per-pass +1.0 added to both x and y is the drop-shadow offset. */

#include <stdint.h>
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/hardware_character_cache.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/blam_data_globals.h"

struct parse_string_state;


extern void cache_hardware_format_character(font_header *font_header, font_character *font_character);
extern void rasterizer_text_draw_character(const dynamic_screen_vertex *vertices);

void rasterizer_draw_character_with_dropshadow(struct parse_string_state *parse_state, font_header *font, font_character *character, unsigned int color, int16_t x0, int16_t y0, int16_t x, int16_t y, int16_t dx, int16_t dy)
{
    cache_hardware_format_character(font, character);
    if ((unsigned __int16)character->hardware_character_index == 0xFFFF)
        return;

    unsigned int shadow_color = global_shadow_color ? global_shadow_color : (color & 0xFF000000);

    for (int shadow_pass = 1; ; shadow_pass = 0)
    {
        float offset = shadow_pass ? 1.0f : 0.0f;
        unsigned int vertex_color = shadow_pass ? shadow_color : color;

        hardware_character *slot = &hardware_character_cache.characters[character->hardware_character_index];
        __int16 u = slot->x0 + x;
        __int16 v = slot->y0 + y;

        dynamic_screen_vertex vertices[4];

        vertices[0].position.n[0] = (float)x0 + offset;       /* top-left */
        vertices[0].position.n[1] = (float)y0 + offset;
        vertices[0].position.n[2] = 0.0f;
        vertices[0].color = vertex_color;
        vertices[0].texcoord.n[0] = (float)u;
        vertices[0].texcoord.n[1] = (float)v;

        vertices[1].position.n[0] = (float)(x0 + dx) + offset; /* top-right */
        vertices[1].position.n[1] = (float)y0 + offset;
        vertices[1].position.n[2] = 0.0f;
        vertices[1].color = vertex_color;
        vertices[1].texcoord.n[0] = (float)(u + dx);
        vertices[1].texcoord.n[1] = (float)v;

        vertices[2].position.n[0] = (float)(x0 + dx) + offset; /* bottom-right */
        vertices[2].position.n[1] = (float)(y0 + dy) + offset;
        vertices[2].position.n[2] = 0.0f;
        vertices[2].color = vertex_color;
        vertices[2].texcoord.n[0] = (float)(u + dx);
        vertices[2].texcoord.n[1] = (float)(v + dy);

        vertices[3].position.n[0] = (float)x0 + offset;        /* bottom-left */
        vertices[3].position.n[1] = (float)(y0 + dy) + offset;
        vertices[3].position.n[2] = 0.0f;
        vertices[3].color = vertex_color;
        vertices[3].texcoord.n[0] = (float)u;
        vertices[3].texcoord.n[1] = (float)(v + dy);

        rasterizer_text_draw_character(vertices);

        if (!shadow_pass)
            break;
    }
}
