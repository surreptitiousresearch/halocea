/* object_globals — runtime object-system state. Layout from the database; the PVS cluster bit-vectors
 * drive per-frame object activation/deactivation in objects_update. */
#pragma once
#include <stdint.h>

/* IDA anonymous union for object_globals.pvs_activation (DB: $60EF112414981E0EE23A1884EE53F811) */
union $60EF112414981E0EE23A1884EE53F811
{
    int     object_index;   /* valid when pvs_activation_type == 1 */
    int16_t cluster_index;  /* valid when pvs_activation_type == 2 */
};

struct object_globals
{
    unsigned char initial_placement;          /* 0x00 */
    unsigned char object_marker_initialized;  /* 0x01 */
    unsigned char force_garbage_collection;   /* 0x02 */
    unsigned char _pad3;                       /* 0x03 */
    int16_t       active_garbage_object_count; /* 0x04 */
    char          _pad6[2];                    /* 0x06 */
    int           first_garbage_object_index;  /* 0x08 */
    unsigned int  last_active_cluster_bits[16];/* 0x0C — previous frame's PVS cluster set */
    unsigned int  active_cluster_bits[16];     /* 0x4C — this frame's combined-player PVS cluster set */
    int           last_garbage_warn_time;      /* 0x8C */
    int16_t       pvs_activation_type;         /* 0x90 — 1=object, 2=cluster, else inactive */
    char          _pad92[2];                   /* 0x92 */
    union $60EF112414981E0EE23A1884EE53F811 pvs_activation; /* 0x94 — 4 bytes (object_index is int) */
};

#ifdef __cplusplus
extern "C" {
#endif

extern struct object_globals *object_globals;
extern int global_object_marker;

#ifdef __cplusplus
}
#endif
