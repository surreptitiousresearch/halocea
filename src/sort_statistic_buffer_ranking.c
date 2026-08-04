/* sort_statistic_buffer_ranking @0x83746430 — qsort comparator for populate_statistic_buffer's
 * statistic==0 combined view: lexicographic by custom (adjusted score) desc, kills desc, deaths asc,
 * assists desc. */

#include "headers/statistic_buffer.h"

int sort_statistic_buffer_ranking(const void *a, const void *b)
{
    const statistic_buffer *buffer_1 = (const statistic_buffer *)a;
    const statistic_buffer *buffer_2 = (const statistic_buffer *)b;

    if (buffer_2->custom < buffer_1->custom)
        return -1;
    if (buffer_2->custom > buffer_1->custom)
        return 1;

    if (buffer_2->kills < buffer_1->kills)
        return -1;
    if (buffer_2->kills > buffer_1->kills)
        return 1;

    if (buffer_2->deaths < buffer_1->deaths)
        return 1;
    if (buffer_2->deaths > buffer_1->deaths)
        return -1;

    if (buffer_2->assists < buffer_1->assists)
        return -1;
    if (buffer_2->assists > buffer_1->assists)
        return 1;

    return 0;
}
