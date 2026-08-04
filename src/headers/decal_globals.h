#pragma once
/* decal_globals — runtime decal bookkeeping. Decals are bucketed into 5 layers, each a per-cluster head
 * index into the global decal datum array (chained through decal+52). Layout from the database. */

struct decal_globals
{
    int first_decal_indices[5][512];        /* 0x0000 — [layer][cluster] head decal index, -1 = empty */
    int first_disconnected_decal_index;     /* 0x2800 — head of decals not attached to a cluster */
    int locked_count;                       /* 0x2804 */
    int permanent_count;                    /* 0x2808 */
};

extern struct decal_globals *decal_globals;
