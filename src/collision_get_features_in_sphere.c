/* collision_get_features_in_sphere @0x837740E0 — top-level query: gather all collision features overlapping a
 * sphere. Tests the world structure collision BSP (when flag 0x20/0x40/0x80 set), optionally converting world
 * hits to features (flag 0x20), then — when flag 0x80 is set — walks the structure leaves the sphere touched,
 * marks their clusters, and for each not-yet-visited collideable object in those clusters recurses through
 * object_get_features_in_sphere. Returns whether any feature was produced.
 *
 * Deviation: float-GPR-skip scrambles the Hex-Rays names (it invents a8/a9/a10; the real feature list is its
 * `a10`/r27). True register map (prologue): r3=flags, r4=center, f1=radius, f2=height, f3=width,
 * r8=ignore_object_index, r9=features. The query radius is expanded by 0.0625. In the object recursion the
 * decompiler put the object's old marker value (`v31`) and the object-header entry (`v29`) into the ignore/features
 * slots; from disasm those are really ignore_object_index and features. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/collision_bsp.h"
#include "headers/structure_bsp.h"
#include "headers/structure_leaf.h"
#include "headers/collision_bsp_test_sphere_result.h"
#include "headers/collision_feature_list.h"
#include "headers/collision_test_flags.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


#include "headers/real_matrix4x3.h"
extern void collision_features_new(collision_feature_list *features);
extern const uint8_t *breakable_surface_flags_get(void);
extern uint8_t collision_bsp_test_sphere(const collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, const real_point3d *center, float radius, collision_bsp_test_sphere_result *result);
extern void collision_bsp_get_features_in_sphere(const collision_bsp *bsp, const collision_bsp_test_sphere_result *result, const real_matrix4x3 *matrix, float height, float width, int object_index, collision_feature_list *features);
extern void structure_cluster_marker_begin(void);
extern void object_marker_begin(void);
extern uint8_t structure_cluster_mark(int16_t cluster_index);
extern int cluster_get_first_collideable_object(int *reference_index, int16_t cluster_index);
extern int cluster_get_next_collideable_object(int *reference_index);
extern void object_marker_end(void);
extern void structure_cluster_marker_end(void);
extern void object_get_features_in_sphere(unsigned int flags, int object_index, const real_point3d *center, float radius, float height, float width, int ignore_object_index, collision_feature_list *features);

uint8_t collision_get_features_in_sphere(unsigned int flags, const real_point3d *center, float radius,
                                                 float height, float width, int ignore_object_index,
                                                 collision_feature_list *features)
{
    collision_features_new(features);

    unsigned int convert_world = flags & (1u << _collision_test_structure_bit);
    if ( (flags & (1u << _collision_test_structure_bit)) != 0
      || (flags & (1u << _collision_test_media_bit)) != 0
      || (flags & (1u << _collision_test_objects_bit)) != 0 )
    {
        structure_bsp *structure = global_structure_bsp;
        float expanded_radius = (float)(radius + 0.0625);
        const collision_bsp *collision = global_collision_bsp;
        const unsigned char *breakable_flags = breakable_surface_flags_get();

        collision_bsp_test_sphere_result result;
        if ( collision_bsp_test_sphere(collision, 256, breakable_flags, center, expanded_radius, &result)
          && convert_world )
        {
            collision_bsp_get_features_in_sphere(collision, &result, (const void *)0, height, width, -1, features);
        }

        if ( ((flags >> _collision_test_objects_bit) & 1) != 0 && result.leaf_count > 0 )
        {
            if ( (flags & _collision_test_objects_all_types_flags) == 0 )
                flags |= _collision_test_objects_all_types_flags;

            /* attested void: all four marker fns take/return nothing; decompiler r3-threading removed */
            structure_cluster_marker_begin();
            object_marker_begin();

            if ( result.leaf_count > 0 )
            {
                int16_t leaf_iter = 0;
                int leaf_index = 0;
                do
                {
                    const structure_leaf *leaf =
                        &((const structure_leaf *)structure->leaves.address)[result.leaf_indices[leaf_index]];
                    int16_t cluster_index = leaf->cluster_index;
                    if ( structure_cluster_mark(cluster_index) )
                    {
                        int reference[4];
                        for ( int object_index = cluster_get_first_collideable_object(reference, cluster_index);
                              object_index != -1;
                              object_index = cluster_get_next_collideable_object(reference) )
                        {
                            object_header_datum *entry =
                                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
                            object_datum *object_data = entry->datum;
                            int old_marker = object_data->object.magic_number;
                            if ( old_marker != global_object_marker )
                            {
                                object_data->object.magic_number = global_object_marker;
                                object_get_features_in_sphere(flags, object_index, center, expanded_radius,
                                                              height, width, ignore_object_index, features);
                            }
                        }
                    }
                    leaf_iter = (int16_t)(leaf_iter + 1);
                    leaf_index = leaf_iter;
                }
                while ( leaf_iter < result.leaf_count );
            }

            object_marker_end();
            structure_cluster_marker_end();
        }
    }

    return features->count[0] || features->count[1] || features->count[2];
}
