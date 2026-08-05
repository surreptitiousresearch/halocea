/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* objects_garbage_collection @ 0x836F23A0 — reclaim object-pool memory and datum slots. First it picks
 * a collection mode from the current pressure on object_memory_pool and the object datum array:
 *   -1 = nothing to do, 0 = forced, 1 = too many active garbage objects (>=50), 2 = pool critically low.
 * (Mode 2 also runs an up-front compaction.) It then snapshots the garbage-object free list (linked
 * through object_data+272) into a stack, and pops it newest-first deleting any object not visible to any
 * player — stopping once the mode-specific target is met (mode 1: active garbage back under 30; mode 2:
 * enough contiguous free space and datum slots recovered). After a compaction, if the garbage list was
 * exhausted without meeting the target it emits a "garbage collection critical" console warning and asks
 * each object_memory_release_procs[] subsystem to release objects until space is recovered.
 *
 * Object data accessed by raw offset; the diagnostic-message block mirrors the decompiler's control flow
 * (the percentage-message format string could not be recovered from the binary — see RAW (irreducible) note). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_globals.h"
#include "headers/memory_pool.h"
#include "headers/object_memory_release_function.h"
#include "headers/object_header_flags.h"
#include "headers/blam_data_globals.h"

extern const object_memory_release_function object_memory_release_procs[3];
extern const char empty_string[]; /* empty string "" */

extern int memory_pool_get_contiguous_free_size(memory_pool *pool);
extern int memory_pool_get_free_size(memory_pool *pool);
extern void memory_pool_compact(memory_pool *pool);
extern uint8_t object_visible_to_any_player(int object_index);
extern void object_set_garbage(int object_index, uint8_t garbage);
extern void object_delete_initial_recursive(int object_index, uint8_t delete_siblings);
extern void object_delete_recursive(int object_index, uint8_t delete_siblings);
extern int game_time_get(void);
extern int sprintf_0(char *string, const char *format, ...);
extern void console_printf(uint8_t clear, const char *format, ...);


void objects_garbage_collection(void)
{
    int16_t mode = -1;

    if ( object_globals->force_garbage_collection )
    {
        mode = 0;
    }
    else if ( memory_pool_get_contiguous_free_size(object_memory_pool) > 104857 )
    {
        if ( 2048 - object_header_data->actual_count > 102 )
        {
            if ( object_globals->active_garbage_object_count >= 50 )
                mode = 1;
        }
        else
        {
            mode = 2;
        }
    }
    else
    {
        memory_pool_compact(object_memory_pool);
        if ( memory_pool_get_contiguous_free_size(object_memory_pool) <= 209715 )
            mode = 2;
    }

    if ( mode == -1 )
    {
        object_globals->force_garbage_collection = 0;
        return;
    }

    /* Snapshot the garbage free list (newest pushed last) into a stack of object indices.
     * (The original aliases this with the 4096-byte release-proc scratch buffer below; kept separate
     * here for clarity — both sized to the same 4096-byte / 1024-entry region.) */
    int garbage_stack[1024];
    int16_t garbage_count = 0;
    int garbage_index = object_globals->first_garbage_object_index;
    if ( garbage_index != -1 )
    {
        do
        {
            garbage_stack[garbage_count++] = garbage_index;
            garbage_index = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, garbage_index)->datum)->object.next_garbage_object_index;
        }
        while ( garbage_index != -1 );
    }

    char target_met = 0;
    while ( 1 )
    {
        if ( mode == 0 )
        {
            target_met = 0;
        }
        else if ( mode == 1 )
        {
            target_met = (object_globals->active_garbage_object_count <= 30);
        }
        else /* mode == 2 */
        {
            target_met = (memory_pool_get_free_size(object_memory_pool) >= 209715
                       && 2048 - object_header_data->count >= 204);
        }

        if ( target_met || garbage_count == 0 )
            break;

        int   object_index = garbage_stack[--garbage_count];
        object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
        char was_active = 1;
        if ( mode == 1 )
            was_active = object_header->flags & (1u << _object_header_active_bit);

        if ( ((uint8_t)((object_visible_to_any_player(object_index) != 0) - 1) & (uint8_t)was_active) != 0 )
        {
            if ( (object_header->flags & (1u << _object_header_active_bit)) != 0 )
                --object_globals->active_garbage_object_count;
            object_set_garbage(object_index, 0);
            object_delete_initial_recursive(object_index, 0);
            object_delete_recursive(object_index, 0);
        }
    }

    memory_pool_compact(object_memory_pool);

    if ( !target_met )
    {
        char wrote_warning = 0;
        const object_memory_release_function *release_proc = object_memory_release_procs;
        char proc_initialized = 0;
        char already_warned_release = 0;
        char buffer_initialized = 0;          /* maps to v15 */
        char status_string[512];
        char release_desc_string[512];
        char warning_string[512];
        unsigned char release_buffer[4096];

        char warn_ready = (object_globals->last_garbage_warn_time == -1
            || object_globals->last_garbage_warn_time + 150 < game_time_get());

        while ( 1 )
        {
            char near_critical = 0;
            char critical = 0;

            if ( mode == 2 )
            {
                int free_size = memory_pool_get_contiguous_free_size(object_memory_pool);
                int slots_free = 2048 - object_header_data->count;
                char use_slots_message = 0;

                if ( free_size > 52428 )
                {
                    if ( slots_free <= 51 )
                    {
                        critical = 1;
                        use_slots_message = 1;
                        near_critical = 1;
                    }
                    else if ( free_size > 104857 )
                    {
                        if ( slots_free > 102 )
                            goto skip_message; /* plenty free — no warning this pass */
                        use_slots_message = 1;
                        near_critical = 1;
                    }
                    else
                    {
                        near_critical = 1;
                    }
                }
                else
                {
                    critical = 1;
                    near_critical = 1;
                }

                if ( use_slots_message )
                    sprintf_0(status_string, "%d slots free", slots_free);
                else
                    /* RAW (irreducible): percentage-message format string stripped/not recoverable from the binary;
                     * value is free_size * 100 / 2097152 (percent of the 2 MB object pool). */
                    sprintf_0(status_string, "%.1f%% free", (double)((float)free_size * 100.0f * 0.00000047683716f));
            }

        skip_message:
            if ( critical || already_warned_release )
            {
                const char *prefix;
                if ( already_warned_release )
                    prefix = critical ? "still " : "not ";
                else
                    prefix = empty_string;
                sprintf_0(warning_string, "garbage collection %scritical (%s)", prefix, status_string);
                console_printf(0, "%s", warning_string);
                wrote_warning = 1;
            }
            else if ( near_critical && warn_ready )
            {
                wrote_warning = 1;
            }

            if ( !critical || !release_proc->function )
            {
                if ( wrote_warning )
                    object_globals->last_garbage_warn_time = game_time_get();
                break;
            }

            uint8_t done = 0;
            while ( !done )
            {
                uint8_t more = 0;
                if ( !buffer_initialized && release_proc->init_function )
                {
                    release_proc->init_function(release_buffer, 4096);
                    buffer_initialized = 1;
                }
                done = release_proc->function(release_desc_string, &more, release_buffer, 4096);
                if ( done )
                {
                    sprintf_0(release_desc_string, "removing objects: %s", release_desc_string);
                    console_printf(0, "%s", release_desc_string);
                }
                if ( !more )
                {
                    ++release_proc;
                    buffer_initialized = 0;
                }
                if ( !release_proc->function )
                {
                    if ( !done )
                    {
                        if ( wrote_warning )
                            object_globals->last_garbage_warn_time = game_time_get();
                        goto done_release;
                    }
                    break;
                }
            }
            already_warned_release = 1;
            memory_pool_compact(object_memory_pool);
        }
    done_release:
        ;
    }

    object_globals->force_garbage_collection = 0;
}
