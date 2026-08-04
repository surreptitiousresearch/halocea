/* netgame_prejoin_players @0x8377EF10 — tail-call forwarder (`b global_network_game_client_get`;
 * confirmed via disasm, not a bare blr). DEVIATION: the raw jump target's own signature
 * (`network_game_client *global_network_game_client_get(void)`) does not match this function's own
 * DB prototype (`void netgame_prejoin_players(widget_instance *widget)`) — a `b` preserves whatever
 * was already in the registers, so `widget` is simply never read by the target, and the target's
 * returned pointer is silently discarded by callers that expect void. Reproduced faithfully: the
 * widget parameter is unused and the call's result is dropped. global_network_game_client_get itself
 * is not yet decompiled — declared extern with its own DB prototype. */

#include "headers/widget_instance.h"
#include "headers/network_game_client.h"

extern network_game_client *global_network_game_client_get(void);

void netgame_prejoin_players(widget_instance *widget)
{
    global_network_game_client_get();
}
