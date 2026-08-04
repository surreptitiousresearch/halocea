/* Chooses a near-square (horizontal x vertical) subframe grid whose cell count is
 * at least num_players, preferring wider-than-tall layouts (increments horizontal
 * first, and only adds a row when the grid is already square-or-wider). */
void compute_subframe_counts(int num_players, unsigned int *out_horizontal_count, unsigned int *out_vertical_count)
{
    unsigned int horizontal = 1;
    unsigned int vertical = 1;

    if (num_players > 1)
    {
        do
        {
            /* (v4^v3)&0x80000000 is always 0 (both stay positive), so the original
             * predicate reduces to: take the ++horizontal branch iff horizontal < vertical */
            if (horizontal < vertical)
                ++horizontal;
            else
            {
                horizontal = 1;
                ++vertical;
            }
        }
        while ((int)(vertical * horizontal) < num_players);
    }

    *out_horizontal_count = horizontal;
    *out_vertical_count = vertical;
}
