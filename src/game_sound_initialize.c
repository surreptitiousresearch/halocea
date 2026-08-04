/* game_sound_initialize @ 0x83713740 — looping-sound pool + game-sound globals */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

typedef struct game_sound_global_data game_sound_global_data;
#include "headers/data_array.h"
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);
extern void *game_state_malloc(const char *name, const char *type, int size);

void game_sound_initialize(void)
{
    game_looping_sound_data = game_state_data_new("object looping sounds", 1024, 52);
    game_sound_globals = game_state_malloc("game sound globals", 0, 12);
}
