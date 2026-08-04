#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* lruv_window — a candidate run of evictable LRU-video pages: where it starts, the newest frame it would
 * evict, and how many pages it has gathered. */

typedef struct lruv_window
{
    int          start_block;       /* block this run starts after */
    unsigned int max_frame;         /* newest frame that would be evicted */
    int          page_offset;       /* first page of the run */
    int          accumulated_pages; /* free/evictable pages gathered so far */
} lruv_window;
