/* hud_picked_up_ammunition @0x83708890 — show the "picked up ammo" HUD message: forwards to
 * hud_add_item_message_server with the picked-up count and message_offset 1 (li r7,1, tail call).
 *
 * attested: void return (tail call to void callee; sole caller ignores r3). Param widths from the
 * pass-through interface: local_player_index lhz+extsh at consumers => int16_t; machine_index lbz at
 * callers (0x836ADF24), never sign-extended => uint8_t; weapon_definition_index stw full word => int;
 * count sth in callee (0x836A5C54) => int16_t. */

#include <stdint.h>

/* attested callee widths: r5 stw (int), r6 sth (int16_t), r7 stb with li r7,-1 caller (int8_t) */
extern void hud_add_item_message_server(int16_t local_player_index, int8_t machine_index, int item_definition_index, int16_t quantity, int8_t message_offset);

void hud_picked_up_ammunition(int16_t local_player_index, uint8_t machine_index, int weapon_definition_index,
                              int16_t count)
{
    hud_add_item_message_server(local_player_index, machine_index, weapon_definition_index, count, 1);
}
