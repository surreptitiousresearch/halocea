/* director_script_camera_record_camera_modes @ 0x82113710 — .rdata, 4 x int16_t (8 bytes, exact).
 * The camera-mode rotation cycle used while a scripted camera path is being recorded; both
 * director_choose_camera and director_choose_camera_script_camera_record pass it to
 * director_rotate_cameras with camera_count == 4. Same cycle as director_game_camera_modes plus
 * the orbiting camera. Raw big-endian halfwords -> resolved meaning:
 *   +0x00 0x0004 -> _camera_first_person
 *   +0x02 0x0002 -> _camera_flying
 *   +0x04 0x0000 -> _camera_following
 *   +0x06 0x0001 -> _camera_orbiting
 */

#include <stdint.h>
#include "../headers/director_camera_mode.h"

const int16_t director_script_camera_record_camera_modes[4] =
{
    _camera_first_person,
    _camera_flying,
    _camera_following,
    _camera_orbiting,
};
