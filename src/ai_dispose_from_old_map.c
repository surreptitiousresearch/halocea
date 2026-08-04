/* ai_dispose_from_old_map @ 0x836E76B8 — per-map AI teardown (reverse of ai_initialize) */
#include "headers/ai_globals.h"
extern void ai_communication_dispose_from_old_map(void);
extern void ai_script_dispose_from_old_map(void);
extern void encounters_dispose_from_old_map(void);
extern void props_dispose_from_old_map(void);
extern void actors_dispose_from_old_map(void);
extern void paths_dispose_from_old_map(void);
void ai_dispose_from_old_map(void)
{
    ai_communication_dispose_from_old_map();
    ai_script_dispose_from_old_map();
    encounters_dispose_from_old_map();
    props_dispose_from_old_map();
    actors_dispose_from_old_map();
    paths_dispose_from_old_map();
    ai_globals->ai_initialized_for_map = 0;
}
