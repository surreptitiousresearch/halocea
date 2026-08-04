/* hcex_cine_term_level @0x823B5420 — per-level teardown of the HCEX cinematic-mask table: drops every
 * script's per-difficulty actor/mask list from `listCine` (the AddCine()-populated
 * script-name -> per-difficulty-actor table registered by hcex_cine_init's ssl callback). */

#include "../headers/hcex/hcex_cine_tick_boundary.h"

void hcex_cine_term_level(void)
{
    dsVECTOR_cine_list_Clear(&listCine);
}
