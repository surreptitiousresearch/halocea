/* cheat_teleport_to_camera_evaluate @0x83729660 — HaloScript builtin evaluator. Takes no script arguments; invokes the
 * engine routine and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; cheat_teleport_to_camera
 * actually takes none, so it is called with its real (void) signature. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void cheat_teleport_to_camera(void);

void cheat_teleport_to_camera_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    cheat_teleport_to_camera();
    hs_return(thread_index, 0);
}
