#pragma once
#include <stdint.h>
/* hs_global_external — an engine-defined (non-scenario) HaloScript global descriptor. Only the leading type
 * field is referenced by the script subsystem; `name` and `address` follow (used by the name lookup). */

typedef struct hs_global_external
{
    const char    *name;          /* 0x00 */
    int16_t        type;          /* 0x04 */
    unsigned char _pad0[2]; /* db-verified padding */
    void          *pointer;       /* 0x08 — backing engine value (null => use the type default) */
    unsigned char  console_flags; /* 0x0C */
} hs_global_external;

#ifdef __cplusplus
extern "C" {
#endif

extern hs_global_external *hs_external_globals[];

#ifdef __cplusplus
}
#endif
/* const: the definition (src/data/hs_external_global_count.c) is `const int16_t ... = 482;` and the
 * object lives at 0x82122214 in .rdata (segment perm 4, read-only); no TU writes it. Without the
 * const this extern is a C2373 "different type modifiers" against the canonical globals header in
 * every TU that includes both — latent only because blam_data_globals.h predates the storage def. */
#ifdef __cplusplus
extern "C" {
#endif

extern const int16_t hs_external_global_count;

#ifdef __cplusplus
}
#endif
