#pragma once
// DB-verified via types_members _4008BC31930BC7182B58823218E2C1A9 (anonymous PDB type)
/* leaf_map_globals — BSP traversal node stack (1032 bytes total, from DB global size). No NAMED DB type
   exists for this block, but an anonymous one does and it carries every member: types_members
   _4008BC31930BC7182B58823218E2C1A9 = node_stack int[256] @0x000, node_stack_count __int16 @0x400,
   __noop char * @0x404. The offsets below are that type's, not an inference from consumers.

   `__noop` at 0x404 is therefore the DB's OWN member name — not, as this comment previously claimed, a
   decompiler-invented idiom. It is an IDA-ism, and it does collide with the MSVC `__noop` intrinsic, but
   the name is layout provenance so it is kept and annotated, exactly as hs_compile_globals.h's identical
   0x01C member is. Role settled by usage: leaf_map_initialize_from_bsp.c:30 returns
   `leaf_map_globals.__noop == 0` as the build's success predicate and every write is an English
   allocation-failure string, i.e. the current error message. */

typedef struct leaf_map_globals_s
{
    int         node_stack[256];   /* 0x000 */
    short       node_stack_count;  /* 0x400 */
    short       _pad402;           /* 0x402 */
    const char *__noop;            /* 0x404 — error message pointer (DB member name kept, see above) */
} leaf_map_globals_s;                /* 0x408 = 1032 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern leaf_map_globals_s leaf_map_globals;

#ifdef __cplusplus
}
#endif
