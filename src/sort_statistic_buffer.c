/* sort_statistic_buffer @0x83746408 — qsort comparator for populate_statistic_buffer's raw single-stat
 * view: descending by `score` alone. */

#include "headers/statistic_buffer.h"

int sort_statistic_buffer(const void *a, const void *b)
{
    const statistic_buffer *buffer_1 = (const statistic_buffer *)a;
    const statistic_buffer *buffer_2 = (const statistic_buffer *)b;

    if (buffer_2->score >= buffer_1->score)
        return buffer_2->score > buffer_1->score;
    return -1;
}
