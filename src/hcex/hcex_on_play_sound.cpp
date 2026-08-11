/* hcex_on_play_sound @0x823E6728 — HCEX bridge hook fired when a Blam sound (`sound_definition_index`) starts
 * playing on unit `unit_index`. If the sound is a dialog line belonging to the current level, it drives the
 * unit's lip-sync: it resolves (or lazily creates) the unit's mdlLIP_SYNCER in the sorted hcexLipSyncList and
 * starts it on the sound's leaf name (the text after the last backslash).
 *
 * The ws template containers (dsVECTOR / dsTSTRING_flat), the mdlLIP_SYNCER animation object and the placement
 * operator new are extern boundaries (see hcex_on_play_sound_boundary.h).
 *
 * NOTE: `impulse_sound_index` is part of the database prototype but is not referenced by the body. */

#include "../headers/hcex/hcex_on_play_sound_boundary.h"

extern "C" void hcex_on_play_sound(int unit_index, int sound_definition_index, int impulse_sound_index)
{
    const char *name = hcex_tag_get_name(sound_definition_index);
    if ( !name )
        return;

    hcex_cine_on_play_sound(name);

    if ( !strstr(name, "dialog") )
        return;
    if ( !strstr(name, haloEngineCtrl->curLevel.pBuffer->str) )
        return;

    char *last_sep = strrchr(name, '\\');
    if ( !last_sep )
        return;
    const char *leaf_name = last_sep + 1;

    HCEX_OBJ *obj = hcex_obj_get(unit_index);
    if ( !IGNORE_STRONG_ASSERT && !obj )
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("obj", "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 2586, empty_string);

    lipsync_pair entry;
    dsCMP cmp;

    entry.key = unit_index;
    cmp = dsCMP();
    int index = dsVECTOR_lipsync_FindSorted(&hcexLipSyncList, &entry.key, &cmp);
    if ( index == -1 )
    {
        mdlLIP_SYNCER *syncer =
            (mdlLIP_SYNCER *)operator new(0x9C, "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 0xA1E);
        if ( syncer )
            mdlLIP_SYNCER_ctor(syncer, obj->pInst);
        else
            syncer = 0;

        entry.val = syncer;
        entry.key = unit_index;
        cmp = dsCMP();
        index = dsVECTOR_lipsync_InsertSorted(&hcexLipSyncList, &entry, &cmp, INS_DUP_IGNORE);
    }

    dsTSTRING_flat leaf_str;
    leaf_str.pBuffer = 0;
    dsTSTRING_UnsafeInit(&leaf_str, leaf_name, -1, 0);

    mdlLIP_SYNCER_Start(dsVECTOR_lipsync_index(&hcexLipSyncList, index)->val, &leaf_str, 0);

    dsTSTRING_BUF_HEADER<char> *buffer = leaf_str.pBuffer;
    if ( --buffer->refCount == 0 )
        dlFree(buffer);
}
