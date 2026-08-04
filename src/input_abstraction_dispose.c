/* input_abstraction_dispose @ 0x83724A00 — zero the input-abstraction globals.
 * Binary is a tail `b memset` (r3 carries memset's return), but the sole caller
 * (game_dispose) ignores r3 — attested void. */

#include "headers/input_abstraction_globals.h"
extern void *memset(void *, int, unsigned int);

void input_abstraction_dispose(void)
{
    memset(&input_abstraction_globals, 0, sizeof(input_abstraction_globals));
}
