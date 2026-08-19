/* main_saving_map @0x83689178 — return whether a map save is currently pending. */

#include <stdint.h>
#include "headers/main_globals.h"

uint8_t main_saving_map(void)
{
    return main_globals.save_map;
}
