/* biped_jump @0x837B2B54 — make a biped jump. Fails if the biped is already airborne (flags bit 0 at
 * +1228) or in a state that forbids jumping (+1288 == 1). The jump magnitude comes from the biped
 * definition (+948); for a player-controlled biped it is scaled by the player jump-height info and the
 * biped's crouch/scale factor (+1060), and quadrupled under the super_jump cheat. The biped's velocity
 * (+104..+112) is projected so its component along the biped up vector (+128..+136) reaches the jump
 * magnitude. A controlling actor (if any) gets to veto/adjust via actor_aim_jump; on success the new
 * velocity is committed, the airborne flag is set, and footsteps are emitted (unless in a simulated
 * update). Returns whether the jump occurred.
 *
 * DEVIATION: the decompiler mis-rendered the actor_aim_jump call via FPR-shadow — the float
 * jump_magnitude reserves r6, so the real 5th arg (r7) is &jump_velocity, not the +1228 flags word
 * the decompiler reused. Corrected from disasm. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/game_globals_player_information.h"
#include "headers/global_tag_instances.h"
#include "headers/game_globals_definition.h"
#include "headers/real_vector3d.h"
#include "headers/biped_datum_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/material_effect_event.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_aim_jump(uint16_t actor_index, int unit_index, uint8_t leap, float jump_magnitude, real_vector3d *jump_velocity);
extern void biped_make_footstep(int biped_index, int16_t event_index, int16_t contact_point_index);

int biped_jump(int biped_index)
{
    biped_datum *biped = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;

    if ((biped->biped.flags & (1u << _biped_airborne_bit)) != 0 || biped->biped.landing == 1)
        return 0;

    int controlling_player = biped->unit.player_index;
    biped_definition *definition = TAG_GET(biped_definition, biped->definition_index);
    float jump_magnitude = definition->biped.jump_velocity;

    /* index [33] into player_information lands on stun_turning_penalty (DB offset 132), scaling by
     * unit.body_stun (+1060) */
    if (controlling_player != -1)
        jump_magnitude = -(((game_globals_player_information *)global_game_globals->player_information.address)
                               ->stun_turning_penalty
                           * biped->unit.body_stun - 1.0f)
                         * definition->biped.jump_velocity;
    if (cheat.super_jump && controlling_player != -1)
        jump_magnitude *= 4.0f;

    real_vector3d jump_velocity;
    jump_velocity.n[0] = biped->object.translational_velocity.n[0];
    jump_velocity.n[1] = biped->object.translational_velocity.n[1];
    jump_velocity.n[2] = biped->object.translational_velocity.n[2];

    const float *up = biped->object.up.n;
    float upward_speed = up[0] * jump_velocity.n[0]
                         + up[1] * jump_velocity.n[1]
                         + up[2] * jump_velocity.n[2];
    if (upward_speed < jump_magnitude)
    {
        float delta = jump_magnitude - upward_speed;
        jump_velocity.n[0] += up[0] * delta;
        jump_velocity.n[1] += up[1] * delta;
        jump_velocity.n[2] += up[2] * delta;
    }

    int actor_index = biped->unit.swarm_actor_index;
    if (actor_index == -1)
        actor_index = biped->unit.actor_index;

    int jumped = 1;
    if (actor_index != -1)
    {
        char state = biped->unit.animation.state;
        unsigned __int8 leap = (state == _unit_state_leap_start || state == _unit_state_leap_airborne);
        jumped = actor_aim_jump(actor_index, biped_index, leap, jump_magnitude, &jump_velocity);
    }

    if ((unsigned char)jumped)
    {
        int flags = biped->biped.flags;
        biped->biped.jump_recovery_timer = 0;
        biped->object.translational_velocity.n[2] = jump_velocity.n[2];
        biped->object.translational_velocity.n[1] = jump_velocity.n[1];
        biped->object.translational_velocity.n[0] = jump_velocity.n[0];
        biped->biped.flags = flags | (1u << _biped_airborne_bit);
        biped->biped.support_surface_index = -1;
        if (!g_running_simulated_update)
        {
            biped_make_footstep(biped_index, _material_effect_biped_jump, 0);
            biped_make_footstep(biped_index, _material_effect_biped_jump, 1);
        }
    }
    return jumped;
}
