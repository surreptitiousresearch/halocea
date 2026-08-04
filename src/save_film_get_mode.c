/* save_film_get_mode @ 0x836EAFA0 — 0=idle, 1=recording, 2=playback (saved_film_mode) */
#include <stdint.h>
#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"
uint16_t save_film_get_mode(void)
{
    /* lhz zero-extend of the int16 mode field; caller re-normalizes with clrlwi 16 => uint16_t return */
    return (uint16_t)global_saved_film.mode;
}
