/* objects_update @ 0x836F2910 — the per-tick object system pass. Three phases:
 *
 *  1. PVS diff: snapshot last frame's active clusters, recompute this frame's combined-player PVS, and
 *     if it changed, walk every object header and (de)activate objects as their cluster enters/leaves the
 *     potentially-visible set. Objects whose cluster left the PVS are either deleted (if flagged to die
 *     out of view) or marked for deactivation; inactive objects whose cluster entered are activated.
 *     Structure decals are refreshed for the changed cluster set.
 *  2. Active update: tick every active, non-frozen object. At double game speed the odd half-tick only
 *     updates objects whose connection-bucket bit is set and that have a valid networked datum.
 *  3. Deferred ops: clear the per-object scratch flags, run a final update for objects flagged for it,
 *     and recursively delete objects flagged for deletion. Ends with a garbage-collection pass.
 *
 * Object header datum is 12 bytes (6 int16): [0] type/salt signature (0 = empty slot), [1] flags,
 * [2] cluster index; the int at byte +8 is the object data pointer. */

#include <stdint.h>
#include "headers/object_globals.h"
#include "headers/object_type.h"
#include "headers/structure_bsp.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_flags.h"
#include "headers/object_header_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"
#include "headers/datum_index.h"


extern int game_time_get(void);
extern uint8_t game_players_are_double_speed(void);
extern unsigned int *players_get_combined_pvs(void);
extern void structure_decals_update(const uint32_t *old_combined_pvs, const uint32_t *new_combined_pvs, int16_t cluster_count);
extern void object_activate(int object_index);
extern void object_delete(int object_index);
extern int object_update(int object_index);
extern void object_delete_recursive(int object_index, uint8_t delete_siblings);
extern void objects_garbage_collection(void);
extern void *memcpy(void *dst, const void *src, unsigned int n);

void objects_update(void)
{
    unsigned int *last_active_cluster_bits = object_globals->last_active_cluster_bits;
    unsigned int *active_cluster_bits      = object_globals->active_cluster_bits;
    int cluster_count;
    unsigned int bitvec_bytes;
    const unsigned int *combined_pvs;
    int pvs_changed;
    int16_t *data;
    int i;
    int last_handle = 0;

    /* phase-2 gate: at double game speed, the odd tick is a half-update */
    char double_speed_half_tick = 0;
    if ( (game_time_get() & 1) != 0 && game_players_are_double_speed() )
        double_speed_half_tick = 1;

    object_globals->active_garbage_object_count = 0;
    cluster_count = (int16_t)global_structure_bsp->clusters.count;
    bitvec_bytes = 4 * BIT_VECTOR_SIZE_IN_LONGS(cluster_count);

    memcpy(last_active_cluster_bits, active_cluster_bits, bitvec_bytes);
    combined_pvs = players_get_combined_pvs();
    memcpy(active_cluster_bits, combined_pvs, bitvec_bytes);

    /* byte-wise compare to detect any PVS change */
    pvs_changed = 0;
    if ( bitvec_bytes )
    {
        unsigned char *a = (unsigned char *)active_cluster_bits;
        unsigned char *l = (unsigned char *)last_active_cluster_bits;
        unsigned char *end = (unsigned char *)&last_active_cluster_bits[bitvec_bytes / 4];
        do
        {
            pvs_changed = *l - *a;
            if ( *l != *a )
                break;
            l++; a++;
        }
        while ( l != end );
    }

    if ( pvs_changed )
    {
        data = (int16_t *)object_header_data->data;
        for ( i = 0; i < object_header_data->count; i++, data += 6 )
        {
            int cluster;
            /* must be a live, connected, activatable object */
            if ( !data[0]
              || (data[1] & (1u << _object_header_automatically_deactivate_bit)) == 0
              || (data[1] & (1u << _object_header_connected_to_map_bit)) == 0 )
                continue;

            if ( (data[1] & (1u << _object_header_active_bit)) == 0 )
            {
                /* currently inactive: activate if its cluster is now visible */
                if ( (data[1] & (1u << _object_header_child_bit)) != 0 )
                    continue;
                cluster = (uint16_t)data[2];
                if ( cluster == 0xFFFF
                  || !BIT_VECTOR_TEST_FLAG(active_cluster_bits, (int16_t)cluster) )
                    continue;
                object_activate(i);
            }
            else
            {
                /* currently active: handle its cluster leaving the PVS */
                if ( !BIT_VECTOR_TEST_FLAG(active_cluster_bits, data[2]) )
                {
                    object_header_datum *record = (object_header_datum *)data;
                    if ( (record->datum->object.flags & (1u << _object_deleted_when_deactivated_bit)) != 0 )  /* dies when out of view */
                    {
                        object_delete((data[0] << 16) | i);
                    }
                    else if ( (record->flags & (1u << _object_header_active_bit)) != 0 )
                    {
                        record->flags &= ~(1u << _object_header_active_bit);                        /* clear active bit */
                    }
                }
            }
        }
        structure_decals_update(last_active_cluster_bits, active_cluster_bits, cluster_count);
    }

    /* phase 2 — tick active objects */
    data = (int16_t *)object_header_data->data;
    for ( i = 0; i < object_header_data->count; i++, data += 6 )
    {
        if ( !data[0] )
            continue;
        if ( (data[1] & (1u << _object_header_active_bit)) != 0
          && (data[1] & (1u << _object_header_being_created_bit)) == 0 )
        {
            int handle = BUILD_DATUM_INDEX(data[0], i);
            last_handle = handle;
            object_header_datum *record = (object_header_datum *)data;
            if ( !double_speed_half_tick
              || (((1 << record->type) & object_mask_unit) != 0
                  && ((unit_datum *)record->datum)->unit.player_index != -1) )  /* types 0/1 are unit-shaped */
            {
                last_handle = object_update(handle);
            }
        }
    }

    /* phase 3 — deferred clear / final-update / recursive-delete */
    data = (int16_t *)object_header_data->data;
    for ( i = 0; i < object_header_data->count; i++, data += 6 )
    {
        if ( !data[0] )
            continue;
        {
            object_header_datum *record = (object_header_datum *)data;
            int needs_final_update = data[1] & (1u << _object_header_being_created_bit);
            unsigned char flags = data[1] & ~(1u << _object_header_do_not_update_bit);
            record->flags = flags;
            if ( needs_final_update )
            {
                record->flags = flags & ~(1u << _object_header_being_created_bit);
                last_handle = object_update(BUILD_DATUM_INDEX(data[0], i));
            }
            if ( (data[1] & (1u << _object_header_being_deleted_bit)) != 0 )
                object_delete_recursive(((uint16_t)data[0] << 16) | i, 0);
        }
    }

    objects_garbage_collection(); /* attested void: r3-thread + phantom arg dropped */
}
