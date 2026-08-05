/* obstacles_disc_neighborhood @0x838300A8 — flood-fill the set of discs transitively touching
 * `seed_disc_index` (two discs "touch" when the distance between their centers is within the sum of their
 * radii, each first padded by `radius`), writing the visited set into `disc_flags` as a bitset (one bit per
 * disc, pre-sized/zeroed for `obstacles->disc_count` bits). A no-op (after the zeroing) if
 * `seed_disc_index == -1`.
 *
 * DEVIATION: the DB prototype confirms only 4 real parameters, with `radius` a `float` (not `double`) that
 * — per the by-now-familiar pattern — reserves its own ABI shadow GPR slot; the raw decompile's own 5th
 * parameter ("disc_flags", shown as unused) is phantom, and what it called `a5` is the DB's real
 * `disc_flags`. The disc addressing (`&obstacles->obstacle_count + 10*idx` typed as `float*`) is exactly
 * `&obstacles->discs[idx]` once the implicit `+8` struct offset is folded back in — verified field-by-field
 * against `headers/obstacles.h`'s already-established `disc` layout via disasm
 * (0x838300A8-0x83830240), and rewritten as plain array indexing. The stack-push value (`v20[v19] = v10`,
 * where `v10` looks like it's reading a stale/previous-iteration register in the raw decompile) is
 * confirmed to equal the current candidate's own loop index at push time — an artifact of the compiler
 * updating that register at the bottom of the loop body, after the push. */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/obstacles.h"

extern void *memset(void *destination, int value, unsigned int size);

static uint8_t disc_flag_test(uint32_t *disc_words, int16_t disc_index)
{
    return BIT_VECTOR_TEST_FLAG(disc_words, disc_index);
}

static void disc_flag_set(uint32_t *disc_words, int16_t disc_index)
{
    BIT_VECTOR_SET_FLAG(disc_words, disc_index);
}

void obstacles_disc_neighborhood(const obstacles *obstacles, float radius, int16_t seed_disc_index, uint8_t *disc_flags)
{
    uint32_t *disc_words = (uint32_t *)disc_flags;  /* 32-bit-word view of the bitset (DB param is uint8_t*) */

    memset(disc_flags, 0, 4 * BIT_VECTOR_SIZE_IN_LONGS(obstacles->disc_count));

    if ( seed_disc_index == -1 )
        return;

    int16_t stack[128];
    int16_t stack_top = 0;

    stack[stack_top++] = seed_disc_index;
    disc_flag_set(disc_words, seed_disc_index);

    while ( stack_top > 0 )
    {
        int16_t current_index = stack[--stack_top];
        const disc *current = &obstacles->discs[current_index];

        for ( int16_t candidate_index = 0; candidate_index < obstacles->disc_count; ++candidate_index )
        {
            if ( disc_flag_test(disc_words, candidate_index) )
                continue;

            const disc *candidate = &obstacles->discs[candidate_index];
            float dx = candidate->center.n[0] - current->center.n[0];
            float dy = candidate->center.n[1] - current->center.n[1];
            float threshold = (candidate->radius + radius) + (current->radius + radius);

            if ( dx * dx + dy * dy <= threshold * threshold )
            {
                disc_flag_set(disc_words, candidate_index);
                stack[stack_top++] = candidate_index;
            }
        }
    }
}
