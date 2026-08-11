/* biped_try_to_make_footsteps @0x837B2D40 — emit footstep effects driven by the biped's current
 * animation. While in a grounded movement state (675 in [2,7]) it either flags an air-contact landing
 * (states 2/3) or measures whether the biped is moving (animation velocity at +632/+636/+640, speed^2
 * > 0.25). With a valid animation (+208) it looks up the animation record (180-byte stride in the
 * animation graph's animations block) whose +64/+65 hold the left/right foot sound frames; when the
 * current frame (+210) hits a foot frame a footstep is made (type depends on +679). Air-contact at
 * frame 0 makes both feet. A separate landing counter (+1234 / +1283) delays the post-jump footsteps a
 * few frames before emitting both feet.
 *
 * DEVIATION: the not-matched branch sets the foot index from an uninitialized stack slot (v14); that
 * path never emits (matched stays false), so it is dropped here. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/global_tag_instances.h"
#include "headers/base_seat.h"
#include "headers/unit_animation_state.h"
#include "headers/material_effect_event.h"
#include "headers/blam_data_globals.h"

extern void biped_make_footstep(int biped_index, int16_t event_index, int16_t contact_point_index);

void biped_try_to_make_footsteps(int biped_index)
{
    bool moving = false;
    char air_contact = 0;
    biped_datum *biped = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;

    unsigned char state = biped->unit.animation.state;
    if (state >= _unit_state_turn_left && state <= _unit_state_move_right) /* turn/move band */
    {
        if (state == _unit_state_turn_right || state == _unit_state_turn_left)
        {
            air_contact = 1;
        }
        else
        {
            /* +632 is unit.throttle (the animation-driving movement input) */
            float vx = biped->unit.throttle.n[0];
            float vy = biped->unit.throttle.n[1];
            float vz = biped->unit.throttle.n[2];
            moving = (vz * vz + (vx * vx + vy * vy)) > 0.25f;
        }
    }

    int animation_index = biped->object.animation.state.index;
    if (animation_index != -1)
    {
        /* the folded 180 was sizeof(animation) */
        animation *animations = (animation *)
            (TAG_GET(animation_graph, biped->object.animation.animation_graph_index))->animations.address;
        animation *animation_record = &animations[animation_index];

        if (air_contact)
        {
            if (biped->object.animation.state.frame_index == 0) /* contact registers on the first frame */
            {
                biped_make_footstep(biped_index, _material_effect_biped_shuffle, 0);
                biped_make_footstep(biped_index, _material_effect_biped_shuffle, 1);
            }
        }
        else if (moving && (animation_record->private_left_foot_frame_index
                            || animation_record->private_right_foot_frame_index))
        {
            int16_t frame = biped->object.animation.state.frame_index;
            char foot = 0;
            bool matched = false;
            if (frame == animation_record->private_left_foot_frame_index)
            {
                foot = 0;
                matched = true;
            }
            else if (frame == animation_record->private_right_foot_frame_index)
            {
                foot = 1;
                matched = true;
            }
            if (matched)
                /* event index: _material_effect_biped_run (1) when standing, else _material_effect_biped_walk (0) */
                biped_make_footstep(biped_index, (uint8_t)biped->unit.animation.base_seat_index == _base_seat_stand, foot);
        }
    }

    /* post-jump landing footstep delay */
    unsigned char landing_flag = biped->biped.state;
    if (landing_flag)
    {
        if (landing_flag == 1)
            biped->biped.stop_ticks = 1;
        else
            biped->biped.stop_ticks = 0;
        return;
    }

    char counter = biped->biped.stop_ticks;
    if (counter > 0)
    {
        char next = counter + 1;
        biped->biped.stop_ticks = next;
        if (next > 3)
        {
            biped_make_footstep(biped_index, _material_effect_biped_shuffle, 0);
            biped_make_footstep(biped_index, _material_effect_biped_shuffle, 1);
            biped->biped.stop_ticks = 0;
        }
    }
}
