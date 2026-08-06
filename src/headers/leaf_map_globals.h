#pragma once
// DB-verified via types_members _4008BC31930BC7182B58823218E2C1A9 (anonymous PDB type)
/* leaf_map_globals — BSP traversal node stack (1032 bytes total, from DB global size; no named DB
   type exists for this block — reconstruction, field offsets confirmed by consumers).
   node_stack[256] at 0x000; node_stack_count at 0x400 confirmed by struct size. */

typedef struct leaf_map_globals_s
{
    int         node_stack[256];   /* 0x000 */
    short       node_stack_count;  /* 0x400 */
    short       _pad402;           /* 0x402 */
    const char *__noop;             /* 0x404 — error message pointer; same decompiler-invented idiom as
                                        hs_compile_globals.__noop (see hs_compile_globals.h) */
} leaf_map_globals_s;                /* 0x408 = 1032 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern leaf_map_globals_s leaf_map_globals;

#ifdef __cplusplus
}
#endif
