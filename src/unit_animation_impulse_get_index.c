/* unit_animation_impulse_get_index @0x836C94E8 — map an AI animation-impulse id (0..13) to its index in a
 * unit's animation-graph "impulse" animation list, and optionally report the interpolation/blend frame count
 * for that impulse. Returns -1 for unknown impulses. */

#include <stdint.h>
#include "headers/unit_animation_impulse.h"
#include "headers/weapon_class_animation.h"

int16_t unit_animation_impulse_get_index(int16_t animation_impulse, int16_t *interpolation_frame_count)
{
    int16_t result = -1;
    switch ( animation_impulse )
    {
        case _unit_animation_impulse_berserk:        result = _weapon_class_animation_berserk;        break;
        case _unit_animation_impulse_signal_move:    result = _weapon_class_animation_signal_move;    break;
        case _unit_animation_impulse_signal_attack:  result = _weapon_class_animation_signal_attack;  break;
        case _unit_animation_impulse_signal_warn:           result = _weapon_class_animation_warn;           break;
        case _unit_animation_impulse_surprise_front: result = _weapon_class_animation_surprise_front; break;
        case _unit_animation_impulse_surprise_back:  result = _weapon_class_animation_surprise_back;  break;
        case _unit_animation_impulse_evade_left:     result = _weapon_class_animation_evade_left;     break;
        case _unit_animation_impulse_evade_right:    result = _weapon_class_animation_evade_right;    break;
        case _unit_animation_impulse_dive_front:     result = _weapon_class_animation_dive_front;     break;
        case _unit_animation_impulse_dive_back:      result = _weapon_class_animation_dive_back;      break;
        case _unit_animation_impulse_dive_left:      result = _weapon_class_animation_dive_left;      break;
        case _unit_animation_impulse_dive_right:     result = _weapon_class_animation_dive_right;     break;
        case _unit_animation_impulse_vehicle_celebrate:      result = _weapon_class_animation_celebrate;      break;
        case _unit_animation_impulse_vehicle_panic:          result = _weapon_class_animation_panic;          break;
        default: break;
    }
    if ( interpolation_frame_count )
    {
        switch ( animation_impulse )
        {
            case _unit_animation_impulse_berserk: case _unit_animation_impulse_signal_move: case _unit_animation_impulse_signal_attack: case _unit_animation_impulse_signal_warn:
            case _unit_animation_impulse_evade_left: case _unit_animation_impulse_evade_right: case _unit_animation_impulse_vehicle_celebrate: case _unit_animation_impulse_vehicle_panic:
                *interpolation_frame_count = 6;
                break;
            case _unit_animation_impulse_surprise_front: case _unit_animation_impulse_surprise_back:
            case _unit_animation_impulse_dive_front: case _unit_animation_impulse_dive_back: case _unit_animation_impulse_dive_left: case _unit_animation_impulse_dive_right:
                *interpolation_frame_count = 3;
                break;
        }
    }
    return result;
}
