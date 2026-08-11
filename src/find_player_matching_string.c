/* find_player_matching_string @0x837663A0 — resolves a console-typed player reference to a network
 * player slot. If the string is all digits/hyphens it is treated as a 1-based player number and matched
 * against the player machine index; otherwise it is treated as a (wide-converted) name and matched
 * against each valid player's name.
 *
 * 0x142..0x542 stride 32 IS network_game_data.players[32] (DB types_members) over a NULL base the compiler
 * folded in: both loop inits (3BE00142 @0x8376642C, 3BE0015E @0x8376649C) and the return `addi r3, r11,
 * 0x142` @0x83766478 carry no base register. Mechanism: see network_game_server_get_game.c. */

#include <stdint.h>
#include "headers/network_player.h"

#include "headers/network_game_server.h"
extern network_game_server *global_network_game_server_get(void);
extern unsigned short *__pctype_func(void);   /* CRT ctype table accessor */
extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);
extern int ustrcmp(const wchar_t *string1, const wchar_t *string2);
extern int  atoi(const char *str);
extern uint8_t network_player_is_valid(network_player *player);

/* str const-qualified 2026-07-30: read-only (classify/widen/atoi), sv_* callers pass const strings (C4090) */
network_player * find_player_matching_string(const char *str)
{
    char is_numeric = 1;
    global_network_game_server_get();   /* threaded-r3 context fetch; result unused here */

    if (str)
    {
        for (const char *p = str; *p; ++p)
        {
            unsigned char c = (unsigned char)*p;
            if ((__pctype_func()[c] & 4) == 0 && c != '-')   /* not a digit and not '-' -> not a number */
            {
                is_numeric = 0;
                break;
            }
        }
    }

    if (!is_numeric)
    {
        wchar_t wide_name[40];
        ascii_to_wide(str, wide_name, 0x1A);
        for (int address = 0x142; address < 0x542; address += 32)
        {
            network_player *player = (network_player *)address;
            if (network_player_is_valid(player) && !ustrcmp(wide_name, player->name))
                return player;
        }
        return nullptr;
    }

    unsigned int player_number = atoi(str) - 1;
    if (player_number < 0x20)
    {
        for (int address = 0x142; address < 0x542; address += 32)
        {
            network_player *player = (network_player *)address;
            if (network_player_is_valid(player) && (unsigned char)player->machine_index == player_number)
                return player;
        }
    }
    return nullptr;
}
