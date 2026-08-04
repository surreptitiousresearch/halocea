#include "headers/blam_data_globals.h"
/* cinematic_initialize @ 0x83690E18 — allocate cinematic globals (28 bytes) */

typedef struct cinematic_globals_definition cinematic_globals_definition;
extern void *game_state_malloc(const char *name, const char *type, int size);

void cinematic_initialize(void)
{
    cinematic_globals = game_state_malloc("cinematic globals", 0, 28);
}
