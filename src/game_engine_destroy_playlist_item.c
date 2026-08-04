#include "headers/game_engine_playlist_item_s.h"

extern void dlFree(void *ptr);

void game_engine_destroy_playlist_item(game_engine_playlist_item_s *item)
{
    dlFree(item->map_name);
    dlFree(item->variant_name);
}
