/* rasterizer_draw_character @0x83768FE8 — draw one already-cached hardware character as a screen-space
 * textured quad. No-ops if the character has no hardware cache slot (hardware_character_index == -1).
 * Otherwise builds the quad's 4 corners: screen position from (x0,y0) to (x0+dx,y0+dy), texture
 * coordinates from the cache slot's (x0,y0) atlas offset plus (x,y) to that plus (dx,dy), all sharing the
 * same packed color; hands the array to rasterizer_text_draw_character.
 *
 * DEVIATION: Hex-Rays fully failed local variable allocation and fabricated a 30-parameter signature; the
 * DB confirms only the first 10 are real (`parse_state` itself is unused by this function — overwritten in
 * its own register before any read). disasm_range(0x83768FE8,0x83769178) resolves the actual local as a
 * 4-element `dynamic_screen_vertex[4]` array that Hex-Rays split into one named struct (`v45`, vertex 0)
 * plus 15 disconnected float/int scalars for vertices 1-3 — reconstructed here by tracing every
 * std/lfd/fcfid/frsp/stfs round trip back to its source integer expression. `position.z` is never written
 * by this function (reproduced as-is, matching the "don't invent" doctrine — the stack slot is whatever was
 * already there). */

#include <stdint.h>
#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/hardware_character_cache.h"
#include "headers/dynamic_screen_vertex.h"

extern void cache_hardware_format_character(font_header *font_header, font_character *font_character);
extern void rasterizer_text_draw_character(const dynamic_screen_vertex *vertices);

void rasterizer_draw_character(parse_string_state *parse_state, font_header *font_header, font_character *font_character, unsigned int color, int16_t x0, int16_t y0, int16_t x, int16_t y, int16_t dx, int16_t dy)
{
    cache_hardware_format_character(font_header, font_character);

    __int16 hardware_character_index = font_character->hardware_character_index;
    if ( hardware_character_index == -1 )
        return;

    hardware_character *character = &hardware_character_cache.characters[hardware_character_index];
    __int16 texel_u0 = character->x0 + x;
    __int16 texel_v0 = character->y0 + y;
    __int16 texel_u1 = texel_u0 + dx;
    __int16 texel_v1 = texel_v0 + dy;

    dynamic_screen_vertex vertices[4];

    vertices[0].position.n[0] = x0;
    vertices[0].position.n[1] = y0;
    vertices[0].color = color;
    vertices[0].texcoord.n[0] = texel_u0;
    vertices[0].texcoord.n[1] = texel_v0;

    vertices[1].position.n[0] = dx + x0;
    vertices[1].position.n[1] = y0;
    vertices[1].color = color;
    vertices[1].texcoord.n[0] = texel_u1;
    vertices[1].texcoord.n[1] = texel_v0;

    vertices[2].position.n[0] = dx + x0;
    vertices[2].position.n[1] = dy + y0;
    vertices[2].color = color;
    vertices[2].texcoord.n[0] = texel_u1;
    vertices[2].texcoord.n[1] = texel_v1;

    vertices[3].position.n[0] = x0;
    vertices[3].position.n[1] = dy + y0;
    vertices[3].color = color;
    vertices[3].texcoord.n[0] = texel_u0;
    vertices[3].texcoord.n[1] = texel_v1;

    rasterizer_text_draw_character(vertices);
}
