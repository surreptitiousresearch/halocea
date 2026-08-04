/* unit_shield_sapping_update @0x836CA210 — while `unit_index` is in animation state 42 ("shield sapping")
 * and its current animation frame index has reached the sap-effect frame (animation record field +46,
 * stride 180, same graph-lookup idiom as biped_try_to_make_footsteps), damage every player unit within 4
 * world units of it (bounding-sphere-center distance) using the unit definition's own `melee_damage` tag.
 * Tracks a consecutive-miss counter at +526: reset to 0 on any hit this tick, incremented otherwise.
 *
 * DEVIATION: the raw decompile reads object+160/164/168 as a plain float triplet that looks like it could be
 * `position`, but per headers/object_datum.h those offsets (accounting for the pointer's leading
 * definition_index dword) land on `bounding_sphere_center`, not `position` — named accordingly here to avoid
 * misleading a future reader.
 *
 * `unit_definition_unit.melee_damage` was newly resolved from the database (types_members on
 * `_unit_definition`) and added to headers/unit_definition.h; every other field in that struct came from the
 * same query. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/damage_data.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);

void unit_shield_sapping_update(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    unit_definition *unit_def = TAG_GET(unit_definition, unit->definition_index);

    if (unit->unit.animation.state == _unit_state_shield_sapping) /* animation state: shield sapping */
    {
        int *animation_graph = TAG_GET(int, unit->object.animation.animation_graph_index);
        animation *animation_record = &((animation *)animation_graph[30])[unit->object.animation.state.index];

        if (unit->object.animation.state.index >= animation_record->private_loop_frame_index)
        {
            unsigned __int8 dealt_damage = 0;
            data_iterator player_iterator;

            data_iterator_new(&player_iterator, player_data);
            for (player_datum *player = data_iterator_next(&player_iterator); player;
                    player = data_iterator_next(&player_iterator))
            {
                if (player->unit_index == -1)
                    continue;

                object_datum *player_unit =
                        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum;

                float dz = unit->object.bounding_sphere_center.n[2] - player_unit->object.bounding_sphere_center.n[2];
                float dx = unit->object.bounding_sphere_center.n[0] - player_unit->object.bounding_sphere_center.n[0];
                float dy = unit->object.bounding_sphere_center.n[1] - player_unit->object.bounding_sphere_center.n[1];

                if (dx * dx + dy * dy + dz * dz < 16.0f)
                {
                    damage_data damage;
                    damage_data_new(&damage, unit_def->unit.melee_damage.index);
                    damage.owner_object_index = unit_index;
                    object_cause_damage(&damage, player->unit_index, -1, -1, -1, nullptr);
                    dealt_damage = 1;
                }
            }

            if (dealt_damage)
                unit->unit.shield_sap_timeout = 0;
            else
                ++unit->unit.shield_sap_timeout;
        }
    }
}
