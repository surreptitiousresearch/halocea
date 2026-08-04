/* object_get_cached_render_state @0x836E6D28 — return the cached render-state index for an object, creating
 * (or recycling) one if the object does not already own a valid cache slot.
 *
 * The object's cached_render_state_index stores its current cache slot. If that slot is still valid (not -1) and
 * still belongs to this object, the existing slot is refreshed and returned. Otherwise a new slot is allocated
 * from the cached_object_render_states pool; if the pool is full, the oldest slot is evicted by scanning every
 * slot and picking the one with the largest age (render.scene_index - slot.scene_index), treating any negative
 * (wrapped) age as the maximum age of 1000. The chosen slot is refreshed and recorded back on the object.
 *
 * DEVIATION: the decompiler invented two phantom parameters (a3, a4) from the soft-float pairing; the true
 * signature takes only (object_index, level_of_detail_pixels). The fourth argument to object_render_state_refresh
 * is a literal: 0 when refreshing an existing slot, 1 when the slot was newly allocated/recycled. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/render_globals.h"
#include "headers/object_header_datum.h"
#include "headers/object_render_state.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int datum_new(data_array *data);
extern int data_next_index(const data_array *data, int16_t index);
/* The 4th parameter (r5) is unused by the callee; the newly-allocated flag is the 5th argument (r6). */
extern void object_render_state_refresh(int cache_index, int object_index, float level_of_detail_pixels, uint8_t unused_dirty, uint8_t force_full_rebuild);

int object_get_cached_render_state(int object_index, float level_of_detail_pixels)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    int cache_index = object->object.cached_render_state_index;

    if ( cache_index != -1
      && DATA_ARRAY_ELEMENT(cached_object_render_states, object_render_state, cache_index)->object_index
             == object_index )
    {
        object_render_state_refresh(cache_index, object_index, level_of_detail_pixels, 0, 0);
        return cache_index;
    }

    int new_index = datum_new(cached_object_render_states);
    if ( new_index == -1 )
    {
        float best_age = -3.4028235e38f;
        for ( int index = data_next_index(cached_object_render_states, -1);
              index != -1;
              index = data_next_index(cached_object_render_states, index) )
        {
            float age = (float)(render.scene_index
                               - DATA_ARRAY_ELEMENT(cached_object_render_states, object_render_state, index)
                                     ->render_scene_index);
            if ( age < 0.0f )
                age = 1000.0f;
            if ( age > best_age )
            {
                best_age = age;
                new_index = index;
            }
        }
        if ( new_index == -1 )
            return new_index;
    }

    object_render_state_refresh(new_index, object_index, level_of_detail_pixels, 0, 1);
    object->object.cached_render_state_index = new_index;
    return new_index;
}
