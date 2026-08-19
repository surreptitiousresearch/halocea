/* hud_update_nav_points @0x837EE3D8 — refresh every active HUD nav-point for every local player. For
 * each player's four nav-point slots, resolve the world position the marker should point at (from its
 * type: 0 = scenario cutscene flag, 1 = tracked object, 2 = game-engine goal), add the slot's vertical
 * offset, then recompute the on-screen render category via hud_get_nav_point_render_type and store it in
 * the slot's packing word. Slots whose tracked object has vanished (or that are otherwise invalid) are
 * deactivated by setting the packing word's top nibble to 0xF.
 *
 * DEVIATION: the decompiler's __ROL4__(packing,16) >> 28 is just packing >> 12 (the type nibble); the
 * goto/LABEL_20 join (types 2 and >=3 skip the per-component position copy) is reproduced structurally. */

#include <stdint.h>
#include "headers/hud_nav_point_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/player_datum.h"
#include "headers/scenario.h"
#include "headers/scenario_cutscene_flag.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/object_type.h"
#include "headers/hud_nav_point_type.h"
#include "headers/blam_data_globals.h"


extern int16_t local_player_get_next(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_point3d *game_engine_get_goal_position(real_point3d *result, int16_t index);
extern int16_t hud_get_nav_point_render_type(int16_t local_player_index, const real_point3d *head, const real_point3d *position, int reference_object_index);

void hud_update_nav_points(void)
{
    int16_t local_player_index = local_player_get_next(-1);

    while (local_player_index != -1)
    {
        hud_nav_point_player_datum *player_nav = &nav_point_data[local_player_index];

        int unit_index;
        if (local_player_get_player_index(local_player_index) == -1)
            unit_index = -1;
        else
            unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
                         local_player_get_player_index(local_player_index))->unit_index;

        for (int j = 0; j < MAXIMUM_ACTIVE_NAV_POINTS; j = (int16_t)(j + 1))
        {
            hud_nav_point_datum *nav_point = &player_nav->nav_points[j];

            if ((uint16_t)nav_point->nav_index == 0xFFFF
                || nav_point->reference_index == -1
                /* recovered: (nav_point->packing & 0xF000) == 0xF000 -> nav_point->type == -1 */
                || nav_point->type == -1)
            {
                /* recovered: nav_point->packing |= 0xF000 -> nav_point->type = -1 (deactivated nibble 0xF) */
                nav_point->type = -1;
                continue;
            }

            if (unit_index == -1)
                continue;

            int reference_object_index = -1;
            real_point3d head_position;
            unit_get_head_position(unit_index, &head_position);

            real_point3d position;
            real_point3d goal_scratch[10];
            /* recovered: nav_point->packing >> 12 -> nav_point->type */
            int type = nav_point->type;

            if (type)
            {
                if (type != _nav_point_object)
                {
                    if ((unsigned int)type < NUMBER_OF_NAV_POINT_TYPES) /* type == 2: game-engine goal */
                    {
                        real_point3d *goal = game_engine_get_goal_position(goal_scratch, nav_point->reference_index);
                        position.n[2] = goal->n[2];
                        position.n[0] = goal->n[0];
                        position.n[1] = goal->n[1];
                    }
                    goto position_ready;
                }

                /* type == 1: tracked object */
                object_datum *object = object_try_and_get_and_verify_type(nav_point->reference_index, object_mask_all);
                reference_object_index = nav_point->reference_index;
                /* disasm reads *(_WORD*)(obj+262): damage_flags, not int[131]. Prior int-index was a mis-scale. */
                if (!object || (object->object.damage_flags & (1u << _object_dead_bit)) != 0)
                {
                    nav_point->reference_index = -1;
                    nav_point->nav_index = -1;
                    /* recovered: nav_point->packing |= 0xF000 -> nav_point->type = -1 (deactivated nibble 0xF) */
                    nav_point->type = -1;
                    continue;
                }
                object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, nav_point->reference_index)->datum;
                position.n[0] = object_data->object.bounding_sphere_center.n[0];
                position.n[1] = object_data->object.bounding_sphere_center.n[1];
                position.n[2] = object_data->object.bounding_sphere_center.n[2];
            }
            else
            {
                /* type == 0: scenario cutscene flag */
                scenario_cutscene_flag *flag =
                    &((scenario_cutscene_flag *)global_scenario->cutscene_flags.address)[nav_point->reference_index];
                position.n[0] = flag->position.n[0];
                position.n[1] = flag->position.n[1];
                position.n[2] = flag->position.n[2];
            }

        position_ready:
            position.n[2] = nav_point->z_offset + position.n[2];
            /* recovered: packing = ((render_type << 8) & 0xF00) | (packing & 0xF0FF) -> screen_type = render_type */
            nav_point->screen_type = hud_get_nav_point_render_type(local_player_index, &head_position, &position,
                                                                   reference_object_index);
        }

        local_player_index = local_player_get_next(local_player_index);
    }
}
