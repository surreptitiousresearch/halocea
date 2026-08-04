/* game_engines @ 0x84185418, 32 bytes (data-init; ledger mislabeled data-bss).
   Table of game-engine variant definitions; slots 0 and 7 are NULL. Pointer
   targets recovered from binary relocations (reloc order authoritative). */
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
