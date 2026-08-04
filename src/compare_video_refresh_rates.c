/* compare_video_refresh_rates @0x83785078 — qsort-style 3-way comparator over refresh-rate values. */

int compare_video_refresh_rates(const unsigned int *ref_1, const unsigned int *ref_2)
{
    if (*ref_1 < *ref_2)
        return -1;
    if (*ref_1 > *ref_2)
        return 1;
    return 0;
}
