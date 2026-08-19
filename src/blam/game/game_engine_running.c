/* game_engine_running @ 0x83747758 — true when a multiplayer engine variant is active */
#include <stdint.h>
extern void *game_engine;
uint8_t game_engine_running(void)
{
    return game_engine != 0;
}
