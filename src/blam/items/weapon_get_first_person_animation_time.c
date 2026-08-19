/* weapon_get_first_person_animation_time @0x836D9490 — duration (in frames) of a first-person weapon animation.
 * Resolves the weapon's first-person animation graph, looks the requested animation type up in the graph's
 * first first-person-weapon animation set (one int16 per type), then reads the timing from the animation
 * definition: frame_count for mode 0, private_key_frame_index for mode 1.
 *
 * Special case: for mode 0 weapons whose definition marks them as pump-action
 * (weapon_type == _weapon_type_shotgun), the shotgun reload animation
 * (_first_person_weapon_animation_shotgun_enter, if present) provides the timing when
 * shotgun_reload_type is 0 or 2.
 *
 * Attested uint16_t return: both return tails end in a bare lhz into r3 (0x836D9558/0x836D9560/0x836D95B4)
 * with NO callee extsh (contrast weapon_rotate_zoom_level's extsh-before-blr); callers re-extend on their
 * side. Params mode/animation_type/shotgun_reload_type are int16_t (extsh r4/r5/r6).
 *
 * BINARY BUG (kept faithful; re-verified instruction-by-instruction 2026-08-10): reload_animation_index
 * reaches -1 two ways — the animation set is shorter than _first_person_weapon_animation_shotgun_enter
 * (cmpwi cr6,r11,0x17 / ble @0x836D9580 -> li r11,-1 @0x836D9594), or its slot 23 holds the -1 sentinel
 * (lhz r10,0x2E(r11) @0x836D9588). Neither is guarded: the element address is formed unconditionally
 * (mulli r10,r11,0xB4 @0x836D9598 = -180, add r10,r10,r9 @0x836D95A0), and r11 is then overwritten with
 * shotgun_reload_type (extsh r11,r6 @0x836D959C), so no index test can survive to the load at
 * 0x836D95B4 — for shotgun_reload_type 0/2 the binary reads animations[-1].frame_count, one element
 * before the animation block, and weapon_magazine_start_reload stores it unclamped (sth r3,2(r26) /
 * sth r3,4(r26) @0x836DC04C).
 * This is an as-built asymmetry, NOT a branch the decompiler dropped: the primary animation_index,
 * produced by the identical table-lookup-or-(-1) idiom, IS guarded 22 instructions earlier
 * (cmpwi cr6,r11,-1 / beqlr cr6 @0x836D9534). The whole function is 75 straight-line instructions with
 * no calls; there is no -1 test anywhere between 0x836D9594 and 0x836D95B4. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_first_person_weapon_animations.h"
#include "headers/animation.h"
#include "headers/weapon_type.h"
#include "headers/first_person_weapon_animation.h"
#include "headers/shotgun_reload_type.h"
#include "headers/blam_data_globals.h"


uint16_t weapon_get_first_person_animation_time(int weapon_index, int16_t mode, int16_t animation_type, int16_t shotgun_reload_type)
{
    uint16_t result = 0;
    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    int graph_tag_index = definition->weapon.interface_definition.first_person_animations.index;
    if ( graph_tag_index == -1 )
        return result;

    animation_graph *graph = TAG_GET(animation_graph, graph_tag_index);
    if ( !graph->first_person_weapon_animations.count )
        return result;
    animation_graph_first_person_weapon_animations *weapon_animations =
        (animation_graph_first_person_weapon_animations *)graph->first_person_weapon_animations.address;
    if ( !weapon_animations )
        return result;

    int16_t animation_index = (animation_type < 0 || animation_type >= weapon_animations->animations.count)
                            ? -1
                            : ((int16_t *)weapon_animations->animations.address)[animation_type];
    if ( animation_index != -1 )
    {
        animation *animations = (animation *)graph->animations.address;
        if ( mode )
        {
            if ( mode == 1 )
                result = (uint16_t)animations[animation_index].private_key_frame_index;
        }
        else
        {
            result = (uint16_t)animations[animation_index].frame_count;
        }

        if ( !mode && definition->weapon.weapon_type == _weapon_type_shotgun )
        {
            int16_t reload_animation_index;
            if ( weapon_animations->animations.count <= _first_person_weapon_animation_shotgun_enter )
                reload_animation_index = -1;
            else
                reload_animation_index = ((int16_t *)weapon_animations->animations.address)
                                             [_first_person_weapon_animation_shotgun_enter];
            /* BINARY BUG: no -1 guard here — contrast the guarded animation_index above
             * (cmpwi cr6,r11,-1 / beqlr cr6 @0x836D9534). animations[-1] is read whenever the
             * shotgun-enter slot is absent (short table) or holds the -1 sentinel. */
            if ( !shotgun_reload_type || shotgun_reload_type == _shotgun_reload_type_first_and_last_round )
                return (uint16_t)animations[reload_animation_index].frame_count;
        }
    }
    return result;
}
