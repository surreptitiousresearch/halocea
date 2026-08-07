/* obstacles_recompute @0x83830248 — clusters the obstacle set's discs into connected "obstacles" via a
 * flood-fill over pairwise distance (two discs join the same obstacle when the gap between their edges,
 * padded by `radius` on both sides, is <=0), then stamps each disc's `obstacle_index` with its cluster id
 * and sets `obstacles->obstacle_count` to the number of clusters found.
 *
 * DEVIATION — same offset-folding decompiler artifact seen in obstacles_add_disc/path_avoid_obstacles: disc
 * base pointers are computed as `(char *)obstacles + 20 * index` (8 bytes short of `&obstacles->discs[index]`),
 * with the missing +8 folded into every subsequent field-offset constant. Verified field-for-field via
 * disasm_range (0x83830268-0x838304E4): folded float-index 4/5/6 = disc.center.x/center.y/radius (absolute
 * offsets 16/20/24 = disc's own 8/12/16 plus the folded array base of 8) at every access site. Reconstructed
 * with direct `disc *` pointers instead of reproducing the folded arithmetic.
 *
 * DEVIATION — `cmpwi cr6, r26, -1` (0x83830318, the decompile's `if (v8 != -1)`) guards entry into the BFS
 * with a comparison against the same register used as the outer disc-index loop counter, which only ever
 * ranges 0..disc_count-1 (disc_count capped at 128) and can never be -1; the branch is unreachable dead code
 * and is omitted below (the BFS is entered unconditionally whenever a disc's `obstacle_index` was still -1).
 *
 * The scratch bit-vector (`visited`, sized `4 * ((disc_count+31)>>5)` = up to 16 bytes for 128 discs) and the
 * scratch queue (`queue`, up to 128 __int16 entries) are both stack scratch buffers local to one cluster-seed
 * pass and are re-zeroed/reused per seed disc — modeled as fixed-size locals matching the 128-disc capacity
 * already established in obstacles.h. */

#include <stdint.h>
#include <string.h>
#include "headers/bit_vector.h"
#include "headers/obstacles.h"

void obstacles_recompute(obstacles *obstacles, float radius)
{
    int disc_count = obstacles->disc_count;
    obstacles->obstacle_count = 0;

    for (int reset_index = 0; reset_index < disc_count; reset_index++)
        obstacles->discs[reset_index].obstacle_index = -1;

    unsigned int visited[4];
    int16_t queue[128];

    for (int seed_index = 0; seed_index < obstacles->disc_count; seed_index++)
    {
        if (obstacles->discs[seed_index].obstacle_index != -1)
            continue;

        int16_t obstacle_index = obstacles->obstacle_count;
        int scan_disc_count = obstacles->disc_count;
        obstacles->obstacle_count++;

        memset(visited, 0, 4 * BIT_VECTOR_SIZE_IN_LONGS(scan_disc_count));

        int queue_count = 1;
        queue[0] = seed_index;
        BIT_VECTOR_SET_FLAG(visited, seed_index);

        while (queue_count > 0)
        {
            const disc *seed_disc = &obstacles->discs[queue[--queue_count]];

            for (int candidate_index = 0; candidate_index < scan_disc_count; candidate_index++)
            {
                int word_index = candidate_index >> 5;
                unsigned int bit = 1 << (candidate_index & 0x1F);
                if (visited[word_index] & bit)
                    continue;

                const disc *candidate_disc = &obstacles->discs[candidate_index];
                float dx = candidate_disc->center.n[0] - seed_disc->center.n[0];
                float dy = candidate_disc->center.n[1] - seed_disc->center.n[1];
                float combined_radius = (candidate_disc->radius + radius) + (seed_disc->radius + radius);

                if (dx * dx + dy * dy <= combined_radius * combined_radius)
                {
                    visited[word_index] |= bit;
                    queue[queue_count++] = candidate_index;
                }
            }
        }

        for (int mark_index = 0; mark_index < obstacles->disc_count; mark_index++)
        {
            if (BIT_VECTOR_TEST_FLAG(visited, mark_index))
                obstacles->discs[mark_index].obstacle_index = obstacle_index;
        }
    }
}
