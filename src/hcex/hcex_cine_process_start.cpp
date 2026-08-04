/* hcex_cine_process_start @0x823B6FF0 — begin the cinematic registered under `script_name` (via the
 * SSL `AddCine(object, name, mask)` callback, see hcex_cine_init): look `script_name` up in `listCine`,
 * pick the per-difficulty actor binding whose mask includes the current difficulty, and start its
 * cinematic. Falls back to the last-registered binding (dsVECTOR::Back) both when no script is
 * registered under that mask AND when the matched binding's actor pointer is null. Called from
 * hcex_start_cine with the calling HaloScript's own name. */

#include "../headers/hcex/hcex_cine_tick_boundary.h"

void hcex_cine_process_start(const dsTSTRING_flat *script_name)
{
    dsCMP cmp = 0;
    int index = dsVECTOR_cine_list_FindSorted(&listCine, script_name, &cmp);
    if ( index == -1 )
        return;

    dsVECTOR_hcexCINE_INFO *masks = &dsVECTOR_cine_list_index(&listCine, index)->val;
    unsigned char difficultyMask = 1 << game_difficulty_level_get();

    struct gsANITEC_ACTOR *actor = 0;
    int matched = 0;

    for ( int i = 0; i < masks->nElem; ++i )
    {
        hcexCINE_INFO *entry = dsVECTOR_hcexCINE_INFO_index(masks, i);
        if ( entry->mask.state & difficultyMask ) // apSTATE_T wraps its value in .state
        {
            actor = entry->actor;
            matched = 1;
            break;
        }
    }

    if ( !matched || !actor )
        actor = dsVECTOR_hcexCINE_INFO_Back(masks)->actor;

    gsANITEC_ACTOR_StartCine(actor);
}
