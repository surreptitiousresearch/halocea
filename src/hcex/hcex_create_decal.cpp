/* hcex_create_decal @0x823E3870 — HCEX bridge hook that records a decal to be spawned by the ws-engine at a
 * bridged Blam world position `position`. Gated on the dbg_hcex_create_decals console toggle. It strips the
 * decal tag name down to its leaf, converts it (hcex_conv_name), rescales/reorients the Blam position into
 * ws space (feet<->metres factor 3.0480001, with Blam Z->ws Y and Blam Y-> negated ws Z), and pushes the
 * resulting HCEX_DECAL onto hcexCreateDecals. When dbg_hcexDumpUsedDecalsOn is set it also logs the
 * "<orig> -> <converted>" name mapping into the sorted hcexUsedDecals set.
 *
 * The ws template containers (dsTSTRING_flat / dsVECTOR) and debug-variable system are extern boundaries
 * (see hcex_create_decal_boundary.h). */

#include "../headers/hcex/hcex_create_decal_boundary.h"

/* Release a ref-counted string buffer: drop the ref, free when it hits zero. Matches the
 * `if (buf->refCount-- == 1) dlFree(buf);` idiom the decompiler emits (post-decrement, old value 1). */
static void tstring_release(dsTSTRING_flat *s)
{
    dsTSTRING_BUF_HEADER<char> *buffer = s->pBuffer;
    if ( buffer->refCount-- == 1 )
        dlFree(buffer);
}

extern "C" void hcex_create_decal(const char *name, hcex_float3 *position)
{
    if ( !dbg_hcex_create_decals.value )
        return;

    const char *leaf_name = name;
    if ( strrchr(name, '\\') )
        leaf_name = strrchr(leaf_name, '\\') + 1;

    char conv_name[64];
    strncpy(conv_name, leaf_name, 0x40);
    conv_name[63] = 0;
    hcex_conv_name(conv_name, 64);

    HCEX_DECAL decal;
    HCEX_DECAL_ctor(&decal);

    dsTSTRING_flat name_str;
    name_str.pBuffer = 0;

    /* Blam (X, Y, Z) -> ws (X, Z, -Y), scaled by the feet/metres factor. */
    decal.pos.x = position->x * 3.0480001f;
    decal.pos.y = position->z * 3.0480001f;
    decal.pos.z = position->y * -3.0480001f;

    dsTSTRING_UnsafeInit(&name_str, conv_name, -1, 0);
    dsTSTRING_assign(&decal.name, &name_str);
    tstring_release(&name_str);

    if ( dbg_hcexDumpUsedDecalsOn.value )
    {
        dsTSTRING_flat mapping;
        mapping.pBuffer = 0;
        dsTSTRING_UnsafeInit(&mapping, leaf_name, -1, 0);
        dsTSTRING_Insert(&mapping, mapping.pBuffer->strLen, " -> ", -1);
        dsTSTRING_Insert(&mapping, mapping.pBuffer->strLen, conv_name, -1);

        dsCMP cmp;
        dsVECTOR_TSTR_InsertSorted(&hcexUsedDecals, &mapping, &cmp, INS_DUP_IGNORE);
        tstring_release(&mapping);
    }

    dsVECTOR_DECAL_PushBack(&hcexCreateDecals, &decal);
    tstring_release(&decal.name);
}
