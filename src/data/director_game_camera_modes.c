/* director_game_camera_modes @ 0x82113708 — .rdata, 3 x int16_t (6 bytes used; the symbol slot
 * runs 8 bytes to the next .rdata symbol, so the trailing zero halfword is alignment padding).
 * The camera-mode rotation cycle director_choose_camera_game hands to director_rotate_cameras
 * (which is called with camera_count == 3). Raw big-endian halfwords -> resolved meaning:
 *   +0x00 0x0004 -> _camera_first_person
 *   +0x02 0x0002 -> _camera_flying
 *   +0x04 0x0000 -> _camera_following
 *   +0x06 0x0000 -> alignment padding (outside the 3 entries the caller walks)
 */

#include <stdint.h>
#include "../headers/director_camera_mode.h"

const int16_t director_game_camera_modes[3] =
{
    _camera_first_person,
    _camera_flying,
    _camera_following,
};
