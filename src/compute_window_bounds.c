/* compute_window_bounds @0x836897A0 — compute the pixel and safe-frame rectangles for one player's
 * split-screen viewport. Players are packed into the smallest rows*cols grid that holds them (columns
 * balanced first); a leftover slot (e.g. 3 players in a 2x2 grid) lets player 0 span two columns while
 * the remaining players shift down a row. The viewport cell is derived from rasterizer_globals.frame_bounds;
 * pixel_bounds is the full cell, and safe_frame_bounds is that cell inset by a 4-pixel gutter on interior
 * edges. Viewports touching a physical screen edge are extended out to rasterizer_globals.screen_bounds so
 * no border shows at the screen perimeter.
 *
 * DEVIATION: the decompiler encoded several zero-tests as `(_cntlzw(x) >> 5) & 1` (== `x == 0`) and the
 * odd-player span as `((_cntlzw(v7) & 0x20) == 0) + 1`; these are written here as direct comparisons. It
 * also reused one register (cell_height's slot) to hold gutter*row_index and copied the pre-gutter rect
 * into pixel_bounds via a fused dword store — reproduced here as explicit field assignments. */

#include "headers/rectangle2d.h"
#include "headers/rasterizer_globals.h"

void compute_window_bounds(int player_index, int num_players,
                           rectangle2d *pixel_bounds, rectangle2d *safe_frame_bounds)
{
    int rows = 1;
    int cols = 1;
    /* 4-pixel safe-frame gutter between viewports (0 for a single player) */
    __int16 gutter = ((num_players >> 31) - (num_players > 1)) & 4;
    unsigned __int8 odd_player_gets_full = 0;

    /* smallest rows*cols grid that holds all players, balancing columns first */
    if (num_players > 1)
    {
        do
        {
            if (((((rows ^ cols) >= 0) + (cols >= (unsigned int)rows)) & 1) != 0)
                ++cols;
            else
            {
                cols = 1;
                ++rows;
            }
        } while (rows * cols < num_players);
    }

    /* leftover slot: shift this player down a row, or let player 0 span the extra column */
    if (rows * cols > num_players)
    {
        if (player_index)
            ++player_index;
        else
            odd_player_gets_full = 1;
    }

    int row_index = player_index / cols;
    int col_index = player_index % cols;
    int full_span = odd_player_gets_full ? 2 : 1;

    __int16 frame_top = rasterizer_globals.frame_bounds.n[0];
    __int16 frame_bottom = rasterizer_globals.frame_bounds.n[2];

    int cell_width = (rasterizer_globals.frame_bounds.n[3] - rasterizer_globals.frame_bounds.n[1]) / cols * full_span;
    int cell_height = (frame_bottom - frame_top) / rows;

    /* full cell rectangle for this player, before gutter inset */
    safe_frame_bounds->n[0] = cell_height * row_index + rasterizer_globals.frame_bounds.n[0];        /* y0 */
    safe_frame_bounds->n[1] = cell_width * col_index + rasterizer_globals.frame_bounds.n[1];         /* x0 */
    safe_frame_bounds->n[2] = (row_index + 1) * cell_height + rasterizer_globals.frame_bounds.n[0];  /* y1 */
    safe_frame_bounds->n[3] = (col_index + 1) * cell_width + rasterizer_globals.frame_bounds.n[1];   /* x1 */

    pixel_bounds->n[0] = safe_frame_bounds->n[0];
    pixel_bounds->n[1] = safe_frame_bounds->n[1];
    pixel_bounds->n[2] = safe_frame_bounds->n[2];
    pixel_bounds->n[3] = safe_frame_bounds->n[3];

    /* inset the safe-frame rectangle by the gutter on interior edges */
    safe_frame_bounds->n[0] += gutter * row_index;
    safe_frame_bounds->n[1] += gutter * col_index;
    safe_frame_bounds->n[2] -= (row_index == 0) ? gutter : 0;
    safe_frame_bounds->n[3] -= (col_index == 0) ? gutter : 0;

    /* viewports on a physical screen edge extend to the full screen bounds */
    if (col_index == 0)
        pixel_bounds->n[1] = rasterizer_globals.screen_bounds.n[1];
    if ((odd_player_gets_full != 0) + col_index + 1 == cols)
        pixel_bounds->n[3] = rasterizer_globals.screen_bounds.n[3];
    if (row_index == 0)
        pixel_bounds->n[0] = rasterizer_globals.screen_bounds.n[0];
    if (row_index + 1 == rows)
        pixel_bounds->n[2] = rasterizer_globals.screen_bounds.n[2];
}
