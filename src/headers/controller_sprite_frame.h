#pragma once
/* controller_sprite_frame — frame index into the shared "controller" UI bitmap sequence: an
 * inactive/active/unknown triple followed by one active/blue/red triple per split-screen player.
 * DB anonymous enum _10AE7F4258F1E889A20AC3A809E6036A (verbatim). */
#ifndef CONTROLLER_SPRITE_FRAME_H
#define CONTROLLER_SPRITE_FRAME_H

enum controller_sprite_frame
{
    _controller_inactive_bitmap = 0,
    _controller_active_bitmap = 1,
    _controller_unknown_bitmap = 2,
    _controller_player1_active_bitmap = 3,
    _controller_player1_blue_active_bitmap = 4,
    _controller_player1_red_active_bitmap = 5,
    _controller_player2_active_bitmap = 6,
    _controller_player2_blue_active_bitmap = 7,
    _controller_player2_red_active_bitmap = 8,
    _controller_player3_active_bitmap = 9,
    _controller_player3_blue_active_bitmap = 10,
    _controller_player3_red_active_bitmap = 11,
    _controller_player4_active_bitmap = 12,
    _controller_player4_blue_active_bitmap = 13,
    _controller_player4_red_active_bitmap = 14,
    NUMBER_OF_CONTROLLER_SPRITE_FRAMES = 15
};

#endif /* CONTROLLER_SPRITE_FRAME_H */
