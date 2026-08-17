#include <string.h>
#include "headers/blam_data_globals.h"
/* ai_initialize @ 0x836E7620 — allocate AI globals (2268 bytes) and init AI subsystems */

#include "headers/ai_globals.h"
extern void *game_state_malloc(const char *name, const char *type, int size);

extern void paths_initialize(void);
extern void actors_initialize(void);
extern void props_initialize(void);
extern void encounters_initialize(void);
extern void ai_script_initialize(void);
extern void ai_communication_initialize(void);
extern void actor_move_initialize(void);

void ai_initialize(void)
{
    ai_globals = game_state_malloc("ai globals", 0, sizeof(ai_globals_t));
    memset(ai_globals, 0, sizeof(ai_globals_t));
    /* attested-void callees (paths/ai_script/actor_move _initialize) break the decompiler
       r3 thread; flattened to a plain sequence (the threaded status is unused). */
    paths_initialize();
    actors_initialize();
    props_initialize();
    encounters_initialize();
    ai_script_initialize();
    ai_communication_initialize();
    actor_move_initialize();
}
