/* main_menu_ensure_player_queues_exist @0x836894B8 — recreate and start the update-server player queues.
 *
 * Deviation: the three update_server_* calls are void no-arg; the decompiler chained them through r3 and
 * typed the function as returning int. Unchained; written as void. */

#include <stdint.h>

extern void update_server_delete(void);
extern uint8_t update_server_new(void);
extern void update_server_start(void);

void main_menu_ensure_player_queues_exist(void)
{
    update_server_delete();
    update_server_new();
    update_server_start();
}
