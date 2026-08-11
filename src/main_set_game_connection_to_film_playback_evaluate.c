/* main_set_game_connection_to_film_playback_evaluate @0x8372BE40 — HaloScript wrapper.
 * Deviation: main_set_game_connection_to_film_playback is void no-arg; hs_* parameters are ABI residue. */

#include <stdint.h>

extern void main_set_game_connection_to_film_playback(void);
extern void hs_return(int thread_index, int value);

void main_set_game_connection_to_film_playback_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    main_set_game_connection_to_film_playback();
    hs_return(thread_index, 0);
}
