/* game_engine_initialize @ 0x8374B408 — set up the multiplayer game engine from a
 * variant and (once) zero the per-player score baseline table.
 *
 * The decompiler renders several writes as raw pointer walks over adjacent globals
 * (a 9-dword clear it anchored to the dummy label algn_846DB304[25..33] — really
 * game_engine_globals itself; and the structure_globals.cluster_magic_numbers tail).
 * These are reproduced faithfully with their literal strides and marked below. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <string.h>
#include "headers/blam_data_globals.h"
#include "headers/game_globals.h"
#include "headers/game_engine_globals.h"

/* memset declared by <string.h> */
extern void game_engine_variant_cleanup(game_variant *variant);

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */
#include "headers/structure_globals.h"
/* DEVIATION: the decompiler modeled structure_globals with cluster_magic_numbers@0; the DB-verified layout
 * has it @8 (after initialized/cluster_marker). The score-baseline clear below is really a raw walk over
 * g_player_score_baselines (disasm: stbu 0x30) that the decompiler attributed to cluster_magic_numbers[502]
 * using the canonical @8 layout — so reusing the canonical header restores the intended address. */

void game_engine_initialize(const game_variant *variant)
{
    int i;
    int *p;

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
        memset(g_player_score_baselines, 0, 384 /* sizeof g_player_score_baselines */);
        /* clear a trailing byte in each of 32 score slots (stride 48 bytes) */
        p = &structure_globals.cluster_magic_numbers[502];
        for ( i = 0; i < 32; ++i )
        {
            p += 12;
            *(unsigned char *)p = 0;
        }
        g_player_score_number_of_used_slots = 0;
        g_player_score_is_initialized = 1;
    }
}
