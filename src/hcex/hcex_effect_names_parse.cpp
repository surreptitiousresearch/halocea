/* hcex_effect_names_parse @0x823E5A20 — load "hcex_effect_names.ps" and populate the sorted
 * hcex_effect_names table (halo effect tag name -> hcex/ws sfx brand name, looked up later by
 * hcex_create_effect) from every top-level section carrying both a "halo" and an "hcex" string key.
 * Sections missing either key, or with an empty value for either, are skipped.
 *
 * deviation: the decompiler's per-entry dsTSTRING_flat/refcount bookkeeping (the InsertSorted entry sharing
 * the already-fetched "halo"/"hcex" string buffers, then releasing the local temporaries) is exactly
 * dsTSTRING_flat's normal share-on-copy convention seen throughout this bridge; reproduced via the same
 * ref-count share pattern used elsewhere rather than the decompiler's literal field-reinterpretation
 * of the psITERATOR/dsPAIR stack slots (confirmed equivalent via the InsertSorted entry's key/val
 * offsets: key <- "halo" string, val <- "hcex" string). */

#include "../headers/hcex/hcex_effect_names_parse_boundary.h"

void hcex_effect_names_parse(void)
{
    dsTSTRING_flat filename = { 0 };
    dsTSTRING_UnsafeInit(&filename, "hcex_effect_names.ps", -1, 0);
    dsTSTRING_flat sub_dir = { 0 };
    dsTSTRING_UnsafeInitEmpty(&sub_dir);
    dsTSTRING_flat ext = { 0 };
    dsTSTRING_UnsafeInitEmpty(&ext);

    psSECTION root = psSYSTEM_LoadPsFromFile(psSystem, filename, sub_dir, ext, 0);

    psITERATOR it;
    psITERATOR_ctor(&it, root, PS_TYPEID_SECTION, 0);

    while ( !psITERATOR_IsDone(&it) )
    {
        psSECTION item = psITERATOR_GetSection(&it);

        dsTSTRING_flat hcex_name = { 0 };
        dsTSTRING_UnsafeInitEmpty(&hcex_name);
        dsTSTRING_flat halo_name = { 0 };
        dsTSTRING_UnsafeInitEmpty(&halo_name);

        psSECTION_INTERFACE_GetDSString(&item, "halo", &halo_name, 0);
        psSECTION_INTERFACE_GetDSString(&item, "hcex", &hcex_name, 0);

        if ( halo_name.pBuffer->strLen && hcex_name.pBuffer->strLen )
        {
            dsPAIR_TSTR entry;
            entry.key = halo_name;
            ++entry.key.pBuffer->refCount;
            entry.val = hcex_name;
            ++entry.val.pBuffer->refCount;

            dsCMP cmp;
            dsVECTOR_PAIR_InsertSorted(&hcex_effect_names, &entry, &cmp, INS_DUP_IGNORE);

            if ( --entry.val.pBuffer->refCount == 0 )
                dlFree(entry.val.pBuffer);
            if ( --entry.key.pBuffer->refCount == 0 )
                dlFree(entry.key.pBuffer);
        }

        if ( --halo_name.pBuffer->refCount == 0 )
            dlFree(halo_name.pBuffer);
        if ( --hcex_name.pBuffer->refCount == 0 )
            dlFree(hcex_name.pBuffer);

        psSection_dtor(&item);
        psITERATOR_Next(&it);
    }

    psITERATOR_dtor(&it);
    psSection_dtor(&root);
}
