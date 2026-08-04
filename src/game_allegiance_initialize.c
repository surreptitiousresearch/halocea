#include "headers/blam_data_globals.h"
/* game_allegiance_initialize @ 0x83745258 — allocate+zero allegiance globals (180 bytes) */

typedef struct struct_game_allegiance_globals struct_game_allegiance_globals;
extern void *game_state_malloc(const char *name, const char *type, int size);
extern void *memset(void *, int, unsigned int);

void game_allegiance_initialize(void)
{
    game_allegiance_globals = game_state_malloc("game allegiance globals", 0, 180);
    memset(game_allegiance_globals, 0, 180);
}
