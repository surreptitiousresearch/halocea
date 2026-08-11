/* structure_decals_update @ 0x837E2E00 — keep per-cluster "permanent" decals in sync with the combined
 * PVS as it changes between frames. For each cluster that has runtime decal records: if it just left the
 * PVS (was in old, not in new, and not a global reset) its permanent decals are deleted; if it just
 * entered the PVS (now visible, and either wasn't before or a reset is pending) its decals are respawned
 * from the cluster's runtime decal records — each record gives an origin, a decal-palette index (→ decal
 * definition tag), and two signed-byte angles converted to a yaw/pitch direction. Clears the reset flag
 * when done. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_runtime_decal.h"
#include "headers/scenario_decal_palette_entry.h"
#include "headers/scenario.h"
#include "headers/structure_decals_globals_definition.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


typedef struct decal_editor_geometry decal_editor_geometry;

#include "headers/real_point3d.h"
extern void decals_delete_permanent_from_cluster(int16_t cluster_index);
extern void decal_new(int definition_index, const real_point3d *origin, const real_vector3d *velocity, float radius_modifier, uint8_t permanent, int16_t forced_sequence_index, decal_editor_geometry *editor_geometry);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);

void structure_decals_update(const uint32_t *old_combined_pvs, const uint32_t *new_combined_pvs, int16_t cluster_count)
{
    structure_bsp *bsp = global_structure_bsp;
    if ( !bsp->runtime_decals.count )
    {
        structure_decals_globals->reset_decals = 0;
        return;
    }
    if ( cluster_count <= 0 )
    {
        structure_decals_globals->reset_decals = 0;
        return;
    }

    for ( int16_t cluster_index = 0; cluster_index < cluster_count; ++cluster_index )
    {
        structure_cluster *cluster = (structure_cluster *)bsp->clusters.address + cluster_index;
        unsigned char has_decals =
            ((uint16_t)cluster->first_runtime_decal_index != 0xFFFF && cluster->runtime_decal_count) ? 1 : 0;

        int word = cluster_index >> 5;
        int bit = 1 << (cluster_index & 0x1F);
        unsigned char was_visible = (old_combined_pvs[word] & bit) != 0;
        unsigned char is_visible = (new_combined_pvs[word] & bit) != 0;

        unsigned char delete_decals =
            has_decals && !structure_decals_globals->reset_decals && was_visible && !is_visible;
        unsigned char spawn_decals =
            has_decals && (!was_visible || structure_decals_globals->reset_decals) && is_visible;

        if ( delete_decals )
        {
            decals_delete_permanent_from_cluster(cluster_index);
        }
        else if ( spawn_decals && cluster->runtime_decal_count )
        {
            int decal_count = cluster->runtime_decal_count;
            for ( int i = 0; i < decal_count; ++i )
            {
                structure_runtime_decal *decal = (structure_runtime_decal *)bsp->runtime_decals.address
                    + cluster->first_runtime_decal_index + i;
                scenario_decal_palette_entry *decal_palette =
                    (scenario_decal_palette_entry *)global_scenario->decal_palette.address;
                uint8_t palette_index = decal->palette_index;
                int definition_index = decal_palette[palette_index].reference.index;

                real_euler_angles2d angles;
                angles.n[0] = (float)decal->yaw * 0.02473695f;
                angles.n[1] = (float)decal->pitch * 0.012368475f;
                real_vector3d direction;
                vector3d_from_euler_angles2d(&direction, &angles);

                /* DEVIATION: decal_new's `float radius_modifier` consumes the r6 GPR slot as well as f1,
                 * so its trailing args are r7/r8/r9 (decal_new never reads r6 — see src/decal_new.c).
                 * 0x837E2FF0-0x837E2FFC sets r9 = 0, r8 = -1, f1 = __real_3f800000 (1.0f) and r7 = 1:
                 * permanent = 1, forced_sequence_index = -1, editor_geometry = NULL. A prior revision
                 * read the decompiler's one-register-left mapping and concluded that r6 (the palette
                 * index loaded by `lbz r6, 0xC(r31)` for the `rotlwi r11, r6, 4` palette lookup two
                 * instructions later) was the `permanent` argument; it is the float's dead shadow slot,
                 * and the 0xFFFFFFFF editor_geometry pointer it produced is dereferenced by
                 * decal_new_from_collision whenever it is non-null. */
                decal_new(definition_index, &decal->position, &direction, 1.0f, 1, -1, nullptr);
            }
        }
    }

    structure_decals_globals->reset_decals = 0;
}
