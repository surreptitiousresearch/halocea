/* player_ui_fast_setup_network_server_evaluate @0x8372DD48 — HaloScript builtin wrapper for
 * player_ui_fast_setup_network_server. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
/* param names/types matched to def (src/player_ui_fast_setup_network_server.c). */
extern void player_ui_fast_setup_network_server(char *map_name, char *variant_name, uint8_t accept_remote_connections);

void player_ui_fast_setup_network_server_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        player_ui_fast_setup_network_server((char *)arguments[0], (char *)arguments[1], ((unsigned char *)arguments)[8]);
        hs_return(thread_index, 0);
    }
}
