/* structure_globals — per-frame structure-cluster marking state. cluster_magic_numbers stamps each
 * cluster with the current marker so a cluster is processed at most once per traversal. */
#pragma once
// DB-verified via types_members _6EB29B45F653492144C800578A4516B8 (anonymous PDB type)

typedef struct structure_globals_t
{
    unsigned char initialized;                /* 0x00 */
    unsigned char cluster_marker_initialized; /* 0x01 */
    unsigned char _pad02[2];                  /* 0x02 */
    int           cluster_marker;             /* 0x04 */
    int           cluster_magic_numbers[512]; /* 0x08 */
} structure_globals_t;

extern structure_globals_t structure_globals;
