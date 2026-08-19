/* game_engine_initialize @ 0x8374B408 — set up the multiplayer game engine from a
 * variant and (once) zero the per-player score baseline table.
 *
 * The decompiler renders several writes as raw pointer walks over ADJACENT globals — it names
 * whichever symbol the displacement happens to land on rather than the one the binary loads.
 * Both are resolved to the real base here and marked below: the 9-dword clear it anchored to the
 * dummy label algn_846DB304[25..33] is game_engine_globals itself, and the score-baseline walk it
 * attributed to structure_globals.cluster_magic_numbers is g_player_score_baselines (r31, taken
 * from `lis/addi g_player_score_baselines` @0x8374B494-98). */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <string.h>
#include "headers/blam_data_globals.h"
#include "headers/game_globals.h"
#include "headers/game_engine_globals.h"

/* memset declared by <string.h> */
extern void game_engine_variant_cleanup(game_variant *variant);

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

void game_engine_initialize(const game_variant *variant)
{
    int i;

    /* zero the whole game_engine_globals block (9 dwords == sizeof, 0x846DB368).
     * DEVIATION: the decompiler anchored this dword-clear to a dummy label 100 bytes
     * earlier and rendered it as algn_846DB304[25..33]; the disasm base is
     * game_engine_globals@ha/@l (stwu loop over &game_engine_globals[0..8]). */
    memset(&game_engine_globals, 0, sizeof(game_engine_globals));

    game_engine_globals.mode = game_engine_mode_active;

    if ( variant && variant->game_engine_index )
    {
        memcpy(&global_variant, variant, sizeof(global_variant));
        game_engine_variant_cleanup(&global_variant);
        game_engine = game_engines[variant->game_engine_index];
    }

    if ( !g_player_score_is_initialized )
    {
        /* DEVIATION: the byte count was 384 under a comment claiming it was sizeof. The binary
         * passes `li r5, 0x600` @0x8374B4AC = 1536 = sizeof(g_player_score_baselines) (32 slots x
         * 48), with r3 = &g_player_score_baselines @0x8374B494-98; 384 cleared a quarter of it. */
        memset(g_player_score_baselines, 0, sizeof(g_player_score_baselines));
        /* DEVIATION: the following stbu walk was anchored to the NEIGHBOURING structure_globals.
         * cluster_magic_numbers[502] is byte 2016 of a 2056-byte object, so a 32 x 48-byte walk ran
         * 1,496 bytes past its end (the two addresses coincide, which is why it worked). The binary
         * forms its base as `addi r11, r31, -0x30` @0x8374B4BC with r31 = &g_player_score_baselines,
         * so the 32 `stbu r30, 0x30(r11)` @0x8374B4C4 clear byte 0 -- is_slot_used -- of each slot.
         * Byte-for-byte the same 32 stores, on the object the binary actually names. */
        for ( i = 0; i < 32; ++i )
            g_player_score_baselines[i].is_slot_used = 0;
        g_player_score_number_of_used_slots = 0;
        g_player_score_is_initialized = 1;
    }
}
