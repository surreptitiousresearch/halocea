/* director_initialize_for_saved_game @ 0x836E5B08 — reinitialize the camera
 * director when resuming from a saved game: run the normal per-map init, then
 * restore the scripted-camera enable state that was persisted in the save. */

#include <stdint.h>
#include "headers/blam_data_globals.h"

extern void director_initialize_for_new_map(void);
extern void director_script_camera(uint8_t enabled);

void director_initialize_for_saved_game(void)
{
    director_initialize_for_new_map();
    director_script_camera(*director_camera_scripted);
}
