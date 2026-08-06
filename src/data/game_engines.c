/* game_engines @ 0x84185418 (.data, 32 bytes)
 * DB applied_types: game_engine *game_engines[8];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x84185438 -> ctf_engine
 *   +0x0008 = 0x841857C8 -> slayer_engine
 *   +0x000C = 0x84185940 -> oddball_engine
 *   +0x0010 = 0x84186AF0 -> king_engine
 *   +0x0014 = 0x84186958 -> race_engine
 *   +0x0018 = 0x841868B8 -> stub_engine
 *   +0x001C = 0x00000000
 * 32 bytes (data-init; ledger mislabeled data-bss).
 *    Table of game-engine variant definitions; slots 0 and 7 are NULL. Pointer
 *    targets recovered from binary relocations (reloc order authoritative).
 */
#include "../headers/game_engine.h"

extern struct game_engine ctf_engine;
extern struct game_engine slayer_engine;
extern struct game_engine oddball_engine;
extern struct game_engine king_engine;
extern struct game_engine race_engine;
extern struct game_engine stub_engine;

struct game_engine *game_engines[8] = {
    0,
    &ctf_engine,
    &slayer_engine,
    &oddball_engine,
    &king_engine,
    &race_engine,
    &stub_engine,
    0,
};
