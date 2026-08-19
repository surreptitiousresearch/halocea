/* encounters_initialize @ 0x837090C8 — encounter/squad/platoon/pursuit pools */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

typedef struct squad_datum squad_datum;
typedef struct platoon_datum platoon_datum;

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);
extern void *game_state_malloc(const char *name, const char *type, int size);

void encounters_initialize(void)
{
    encounter_data = game_state_data_new("encounter", 128, 108);
    squad_array = game_state_malloc("squad", "squad", 0x8000);
    platoon_array = game_state_malloc("platoon", "platoon", 4096);
    pursuit_data = game_state_data_new("ai pursuit", 256, 40);
}
