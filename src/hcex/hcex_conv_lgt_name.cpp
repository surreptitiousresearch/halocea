/* hcex_conv_lgt_name @0x823F2CA8 — build the HCEX key for a light: the literal prefix "lgt_" followed
 * by the tag path's leaf component (the text after the final '\'), normalized by hcex_conv_name before
 * return. Returns `name` unchanged (nullptr passthrough) when no name is given.
 *
 * The decompiler renders the "lgt_" prefix as a manual byte-copy loop out of an unnamed alignment
 * blob (algn_82005095); the DB bytes at that address (0x82005098-0x8200509C) read 'l','g','t','_',0,
 * so the loop is reproduced here as the literal it actually copies. The leaf-name append is the
 * inlined strcpy the compiler emitted for `strcpy(out + 4, strrchr(name, '\\') + 1)`.
 *
 * strrchr is a CRT boundary; hcex_conv_name is an HCEX sibling. */

#include <string.h>

extern char *hcex_conv_name(char *name, int max_len);

extern "C" char *hcex_conv_lgt_name(const char *name, char *out, int max_len)
{
    if ( !name )
        return (char *)name;

    strcpy(out, "lgt_");
    strcpy(out + 4, strrchr(name, '\\') + 1);

    return hcex_conv_name(out, max_len);
}
