#include <stdint.h>

extern int16_t game_time_get_elapsed(void);

/* DEVIATION: disasm: extsh r3, r3 = sign-extend 16-bit return value */
extern "C" int hcex_get_elapsed_tics(void)
{
    return (short)game_time_get_elapsed();
}
