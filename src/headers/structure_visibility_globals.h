#pragma once
// DB-verified via types_members _00E31829F1E47A35611435DD278C8858 (anonymous PDB type)
/* structure_visibility_globals — scratch state for the per-frame structure visibility pass. visited_cluster_
 * flags points at a caller-supplied bit vector used to dedupe clusters during the traversal flood-fill;
 * rendered_cluster_indices is indexed by BSP cluster index and holds that cluster's slot in
 * render.rendered_clusters[] for this frame (bounded by visible_cluster_flags[16] = 512 clusters). */

/* Shadow pattern: object name == type name → bare struct TAG only (no typedef). */
struct structure_visibility_globals
{
    unsigned int *visited_cluster_flags;         /* 0x00 */
    __int16       rendered_cluster_indices[512]; /* 0x04 */
};

extern struct structure_visibility_globals structure_visibility_globals;
