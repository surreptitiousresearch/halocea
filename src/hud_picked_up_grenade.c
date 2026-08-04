/* hud_picked_up_grenade @0x83708880 — show the "picked up grenade" HUD message: forwards to
 * hud_add_item_message_server with quantity 1 and message_offset -1 (li r6,1 / li r7,-1, tail call).
 *
 * attested: void return (tail call to void callee; sole caller ignores r3). Param widths from the
 * pass-through interface: local_player_index lhz+extsh at consumers => int16_t; machine_index lbz at
 * callers (0x836ADFA4), never sign-extended => uint8_t; grenade_definition_index stw full word => int. */

#include <stdint.h>

/* attested callee widths: r5 stw (int), r6 sth (int16_t), r7 stb with li r7,-1 caller (int8_t) */
extern void hud_add_item_message_server(int16_t local_player_index, int8_t machine_index, int item_definition_index, int16_t quantity, uint8_t message_offset);

void hud_picked_up_grenade(int16_t local_player_index, uint8_t machine_index, int grenade_definition_index)
{
    hud_add_item_message_server(local_player_index, machine_index, grenade_definition_index, 1, -1);
}
