/* object_memory_release_procs @ 0x821139B4 (.rdata, 24 bytes)
 * DB applied_types: const object_memory_release_function object_memory_release_procs[3];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   [ 0] +0x00 init_function              = 0x00000000
 *        +0x04 function                   = 0x836E7A68 -> ai_release_inactive_swarms
 *   [ 1] +0x00 init_function              = 0x836E7B68 -> ai_find_inactive_encounters
 *        +0x04 function                   = 0x836E7D30 -> ai_release_inactive_encounters
 *   [ 2] +0x00 init_function              = 0x00000000
 *        +0x04 function                   = 0x00000000
 * .rdata, 24 bytes = 3 x object_memory_release_function
 * (sizeof == 8; 24 / 8 == 3, no padding). The table objects_garbage_collection walks when a
 * compaction still leaves the object pool critically low: each subsystem is asked to release the
 * objects it owns, and the walk ends at the first entry whose `function` slot is null.
 * Initializer reconstructed from the binary relocations (big-endian):
 *   [0] init_function = 0x00000000 -> (none)
 *       function      = 0x836E7A68 -> ai_release_inactive_swarms
 *   [1] init_function = 0x836E7B68 -> ai_find_inactive_encounters
 *       function      = 0x836E7D30 -> ai_release_inactive_encounters
 *   [2] init_function = 0x00000000, function = 0x00000000  -> terminator
 * /
 */
#include <stdint.h>
#include "../headers/object_memory_release_function.h"

/* Both release callbacks are declared with the funcptr-SLOT signature taken from
 * object_memory_release_function, not the concrete spelling of their definition TUs
 * (ai_release_inactive_swarms returns unsigned int, ai_release_inactive_encounters returns int);
 * the concrete return types mismatch the slot and would raise C4113. */
extern void    ai_find_inactive_encounters(unsigned char *buffer, int16_t buffer_size);                                      /* 0x836E7B68 */
extern uint8_t ai_release_inactive_swarms(char *description, uint8_t *more, unsigned char *buffer, int16_t buffer_size);     /* 0x836E7A68 */
extern uint8_t ai_release_inactive_encounters(char *description, uint8_t *more, unsigned char *buffer, int16_t buffer_size); /* 0x836E7D30 */

/* Field order per row (object_memory_release_function, DB-verified layout):
   init_function, function. */
const object_memory_release_function object_memory_release_procs[3] =
{
    { nullptr,                     ai_release_inactive_swarms     },
    { ai_find_inactive_encounters, ai_release_inactive_encounters },
    { nullptr,                     nullptr                        },  /* terminator: !function ends the walk */
};
