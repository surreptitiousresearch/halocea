/* weapon_effect_new @0x836D9C30 — spawn an effect (or impulse sound) on behalf of a weapon. The effect is
 * created from the weapon's owning object; if the weapon is attached/parented (object flags bit 0) the
 * effect's marker source is the parent object instead of the weapon itself.
 *
 * The tag group of `effect_index` selects the path:
 *   - 'effe' (effect, 1701209701): create a full effect via effect_new_from_object, briefly recording the
 *     owning weapon in the global weapon_effect_owner so the effect system can attribute it.
 *   - 'snd!' (sound, 1936614433): spawn an impulse sound, flagged for local-player playback when the
 *     effect's owning unit belongs to a local player.
 *   - anything else: ignored (return -1).
 *
 * DEVIATION: the decompiler placed the effect_new_from_object color/impulse_field args in r7/r8; the
 * PPC float-skip ABI actually puts them in r9/r10, and the disassembly shows both are 0 (NULL). Likewise
 * the impulse-sound "is_local_player" arg is the computed local-player flag (r9), not the 0x82000000
 * artifact the decompiler reported. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/effect_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/object_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern int weapon_get_owner_object_index(int weapon_index);
extern uint32_t tag_get_group_tag(int16_t tag_index);
extern effect_datum *effect_try_and_get(int index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void *datum_try_and_get(const data_array *data, int index);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);

int weapon_effect_new(int weapon_index, int effect_index, float effect_scale, float effect_error)
{
    if ( effect_index == -1 )
        return -1;

    int marker_object_index = weapon_index;
    object_datum *weapon =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    if ( (weapon->object.flags & (1u << _object_invisible_bit)) != 0 )            /* attached: emit from the parent object's marker */
    {
        int parent_object_index = weapon->object.parent_object_index;
        if ( parent_object_index != -1 )
            marker_object_index = parent_object_index;
    }

    int owner_object_index = weapon_get_owner_object_index(weapon_index);
    unsigned int group_tag = tag_get_group_tag(effect_index);

    if ( group_tag != 0x65666665u )               /* not 'effe' (effect) */
    {
        if ( group_tag == 0x736E6421u )           /* 'snd!' : impulse sound */
        {
            unsigned __int8 is_local_player = 0;
            effect_datum *effect = effect_try_and_get(effect_index);
            if ( effect )
            {
                unit_datum *owner = object_try_and_get_and_verify_type(effect->owner_object_index, object_mask_unit);
                if ( owner )
                {
                    player_datum *player = datum_try_and_get(player_data, owner->unit.player_index);
                    if ( player && player->local_player_index != -1 )
                        is_local_player = 1;
                }
            }
            object_impulse_sound_new(owner_object_index, effect_index, -1, global_origin3d, global_forward3d,
                                     effect_scale, is_local_player);
        }
        return -1;
    }

    weapon_effect_owner = weapon_index;
    int result = effect_new_from_object(effect_index, owner_object_index, marker_object_index, -1,
                                        effect_scale, effect_error, 0, 0);
    weapon_effect_owner = -1;
    return result;
}
