/* game_engine_post_rasterize_objects @0x83746910 — let the active multiplayer game mode run its
 * post-rasterize hook, if any. Dispatches through the game_engine vtable's post_rasterize slot. */

#include "headers/game_engine.h"


void game_engine_post_rasterize_objects(void)
{
    if (!game_engine)
        return;
    if (!game_engine->post_rasterize)
        return;
    game_engine->post_rasterize();
}
