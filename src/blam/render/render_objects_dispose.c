/* render_objects_dispose @0x836E5CA0 — one-time teardown of the object render-state cache: drop the global
 * pointer (the pool itself lives in the game-state arena). */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


void render_objects_dispose(void)
{
    cached_object_render_states = 0;
}
