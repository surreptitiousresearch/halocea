#include "headers/blam_data_globals.h"
/* first_person_weapons_initialize @ 0x8369DD78 — allocate FP weapon state (15680 bytes) */

typedef struct first_person_weapon first_person_weapon;
extern void *game_state_malloc(const char *name, const char *type, int size);

void first_person_weapons_initialize(void)
{
    first_person_weapons = game_state_malloc("first person weapons", 0, 15680);
}
