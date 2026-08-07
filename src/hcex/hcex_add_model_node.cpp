/* hcex_add_model_node @0x823E0970 — hcex bridge: append a named node to an HCEX model's node-name list.
 * The model object (`mdl`) carries a dsVECTOR<dsTSTRING<char>,8> at byte offset 0x0C; this wraps node_name
 * in a ref-counted dsTSTRING_flat, pushes a copy into that vector, then drops the local string's reference
 * (freeing its buffer if this was the last owner). ds container ops are ws-engine boundaries. */

#include "../headers/hcex/hcex_ds_boundary.h"

/* DEVIATION: `mdl` was `char *`. DB funcs hcex_add_model_node says `void *mdl` (the sibling
 * hcex_add_model_reg already spells it that way; hcex_add_model_perm's mangled symbol
 * `?hcex_add_model_perm@@YAXPAXHPBD@Z` carries PAX = void* for the same slot). */
extern "C" void hcex_add_model_node(void *mdl, const char *node_name)
{
    dsTSTRING_flat node; node.pBuffer = nullptr;

    dsTSTRING_UnsafeInit(&node, node_name, -1, 0);
    dsVECTOR_TSTRING_PushBack((char *)mdl + 12, &node); /* mdl+0x0C: dsVECTOR<dsTSTRING<char>,8> */

    if ( node.pBuffer->refCount-- == 1 )
        dlFree(node.pBuffer);
}
