/* effect_new_attached_from_markers @0x836E40D8 — spawn an effect attached to a set of named markers on an
 * object (or an explicit point/forward list). Mirrors the effect into the hcex render layer, allocates the
 * effect datum, seeds its scale/color/impulse-field, tags it non-violent when attached to a corpse, builds
 * its per-marker locations from the resolved object node matrix, and runs the first update. Returns the new
 * effect index, or -1 on allocation failure.
 *
 * DEVIATION: the two float args (scale_a/scale_b) consume GPR slots, so the decompiler shifts the trailing
 * pointer args into phantoms — the real color is its `a34` and the real impulse_field its `a36`; the clean
 * 12-argument signature is from the database. */

#include <stdint.h>
#include "headers/effect_datum.h"
#include "headers/effect_marker_list.h"
#include "headers/effect_vector_field.h"
#include "headers/real_rgb_color.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/effect_flags.h"
#include "headers/blam_data_globals.h"

struct object_marker;


#include "headers/object_marker.h"
#include "headers/object_marker.h"
#include "headers/object_marker.h"
#include "headers/data_array.h"
extern void hcex_init_effect(int definition_index, int obj_follow, int plr_idx, const real_point3d *points,
                             const real_vector3d *vectors, const char **names, int npoints, float scale);
extern void *datum_get(data_array *data, int index);
extern int effect_allocate(int definition_index, int owner_object_index, uint8_t can_be_deterministic);
extern uint8_t effects_object_is_corpse(int object_index);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern void *memset(void *dst, int value, unsigned int size);
extern int16_t effect_marker_list_get_markers_by_name(int object_index, const char *name, object_marker *results, int16_t maximum_count);
extern void effect_build_locations(effect_datum *effect, int16_t (*get_markers_by_name)(int, const char *, object_marker *, int16_t));
extern void effect_update(int effect_index, float dt);

extern void impulse_effect_initialize(effect_datum *effect, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
int effect_new_attached_from_markers(int definition_index, int owner_object_index, int object_index,
                                     int16_t node_index, int16_t marker_count, const char **marker_names,
                                     const real_point3d *marker_points, const real_vector3d *marker_forwards,
                                     float scale_a, float scale_b, const real_rgb_color *color,
                                     const effect_vector_field *impulse_field)
{
    hcex_init_effect(definition_index, object_index, -1, marker_points, marker_forwards, marker_names,
                     marker_count, scale_a);

    int effect_index = effect_allocate(definition_index, owner_object_index, 1u);
    if (effect_index != -1)
    {
        /* recovered: effect_data->data + 252*(u16)effect_index -> datum_get (252 == effect datum_size incl. 2-byte salt header) */
        effect_datum *effect = datum_get(effect_data, effect_index);
        /* DEVIATION: collapsed verbatim-inlined body of impulse_effect_initialize@0x836E15E0 (zero-xref donor) back into a call; args pass through unchanged, no folding needed at this call site. */
        impulse_effect_initialize(effect, scale_a, scale_b, color, impulse_field);

        effect->object_index = object_index;
        if (effects_corpse_nonviolent && effects_object_is_corpse(object_index))
            effect->flags |= (1u << _effect_nonviolent_bit);

        effect_marker_list marker_list;
        marker_list.count = marker_count;
        marker_list.names = marker_names;
        marker_list.points = marker_points;
        marker_list.forwards = marker_forwards;
        marker_list.node_index = node_index == -1 ? 0 : node_index;
        marker_list.node_matrix = object_get_node_matrix(object_index, marker_list.node_index);
        global_effect_marker_list = &marker_list;

        memset(effect->location_datum_indices, -1, sizeof(effect->location_datum_indices));
        effect_build_locations(effect, effect_marker_list_get_markers_by_name);
        effect_update(effect_index, 0.0);
    }
    return effect_index;
}
