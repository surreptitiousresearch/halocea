/* director_save_camera @0x836E4630 — debug helper: write local player 0's current camera (position, forward,
 * up, field of view) to "d:\camera.txt" as plain text, for later reload via director_load_camera. */

#include <stdint.h>
#include <stdio.h>
#include "headers/observer_result.h"

extern const observer_result *observer_get_camera(int16_t local_player_index);

/* DEVIATION: the decompiler threaded fclose's status through r3 and typed the function as returning
 * FILE * (via an intptr_t pun). Disasm shows the two exit paths leave unrelated values in r3 (fopen's
 * NULL on the early-out, fclose's status on the success path) and the single caller
 * (director_save_camera_evaluate @0x8372B5D4) ignores r3 — the function returns void. */
void director_save_camera(void)
{
    FILE *file = fopen("d:\\camera.txt", "w");
    if ( file )
    {
        const observer_result *camera = observer_get_camera(0);
        fprintf(file, "%f %f %f\n", camera->position.n[0], camera->position.n[1], camera->position.n[2]);
        fprintf(file, "%f %f %f\n", camera->forward.n[0], camera->forward.n[1], camera->forward.n[2]);
        fprintf(file, "%f %f %f\n", camera->up.n[0], camera->up.n[1], camera->up.n[2]);
        fprintf(file, "%f\n", camera->field_of_view);
        fclose(file);
    }
}
