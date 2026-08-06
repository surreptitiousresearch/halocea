/* director_choose_game_perspective @0x836E504C — picks the camera perspective (first-person vs
 * following) for a local player's director, based on what the player's unit currently wants. When not
 * forced, it only re-initializes the camera if switching away from the opposite perspective, and it
 * starts a brief camera-change pause. Always latches the desired seat state. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/following_camera.h"
#include "headers/first_person_camera.h"
#include "headers/director_perspective.h"
#include "headers/blam_data_globals.h"

extern int player_control_get_unit_index(int16_t local_player_index);
extern int16_t director_desired_perspective(int unit_index, int16_t *seat_state);
extern void following_camera_new(following_camera *camera);
/* declared __fastcall to match director.camera_proc's type, so the funcptr compares/assigns below need no cast */
extern void following_camera_update(following_camera *camera, const camera_control *controls, observer_command *result);
extern void first_person_camera_new(first_person_camera *camera);
extern void first_person_camera_update(first_person_camera *camera, const camera_control *controls, observer_command *result);

extern void director_set_camera(int16_t local_player_index, void (*camera_proc)(void *camera_data, const camera_control *, observer_command *), uint8_t interpolate);
void director_choose_game_perspective(int16_t local_player_index, uint8_t force)
{
    director *dir = &director_globals.local_players[local_player_index];
    int unit_index = player_control_get_unit_index(local_player_index);

    int16_t desired_seat;
    int16_t perspective = director_desired_perspective(unit_index, &desired_seat);

    if (force || dir->seat_state != desired_seat)
    {
        void (*new_proc)(void *, const camera_control *, observer_command *);

        if (perspective == _director_perspective_third_person)  /* following (3rd-person) camera */
        {
            /* Only switch in when currently in the first-person camera (unless forced). */
            if (!force && dir->camera_proc != first_person_camera_update)
                goto set_seat;
            following_camera_new((following_camera *)dir->camera_data);
            new_proc = following_camera_update;
        }
        else  /* first-person camera */
        {
            if (!force && dir->camera_proc != following_camera_update)
                goto set_seat;
            first_person_camera_new((first_person_camera *)dir->camera_data);
            new_proc = first_person_camera_update;
        }

        /* DEVIATION: director_set_camera@0x836E48E8 inlined here (zero-xref donor); interpolate arg is the host's own !force value (not compile-time constant since force is a caller-supplied parameter) — collapsed to a call. */
        director_set_camera(local_player_index, new_proc, !force);

    set_seat:
        dir->seat_state = desired_seat;
    }
}
