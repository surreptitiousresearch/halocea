#pragma once
// DB-verified via types_members _303708F16CCD5B5D084E785631C2CA68 (anonymous PDB type)
/* hs_compile_globals — HaloScript compiler state. Layout taken verbatim from the database anonymous struct
 * type (_303708F16CCD5B5D084E785631C2CA68); field names and offsets are authoritative.
 *
 * `__noop` at 0x01C is the DB's OWN member name, not a placeholder this reconstruction chose:
 * types_members $303708F16CCD5B5D084E785631C2CA68 member_index 7 is literally `__noop`, `const char *`,
 * and IDA prints it that way in the code too (`stw r11, $303708F1....__noop(r30)` @0x83776B0C in
 * hs_parse_ai, @0x83778C78 in hs_parse_begin). It IS an IDA-ism and it DOES collide with the MSVC
 * `__noop` intrinsic, but the name is layout provenance, so it is kept and annotated here — the same
 * decision antenna_datum.h, flag_datum.h and trigger_create_projectiles.c:240 already record for the other
 * three DB members spelled `__noop`. (The projectile_effect_new.c precedent renames a PARAMETER, whose
 * name carries no layout evidence; a member is not that.) Its role is settled by its DB neighbours
 * error_since_initialize@0x18 / error_offset@0x20 / error_buffer@0x24 and by hs_compile.c:64, which uses
 * `== nullptr` on it as the whole compiler's success predicate: it is the current error message. */

typedef struct hs_compile_globals_t
{
    unsigned char  initialized;                   /* 0x000 */
    unsigned char  _pad001[3];                    /* 0x001 */
    int            compiled_source_size;          /* 0x004 */
    char          *compiled_source;               /* 0x008 */
    char          *string_constant_buffer;        /* 0x00C */
    int            string_constant_buffer_offset; /* 0x010 */
    int            string_constant_buffer_size;   /* 0x014 */
    unsigned char  error_since_initialize;        /* 0x018 */
    unsigned char  _pad019[3];                    /* 0x019 */
    const char    *__noop;                        /* 0x01C — error message pointer */
    int            error_offset;                  /* 0x020 */
    char           error_buffer[256];             /* 0x024 */
    unsigned char  malloced;                      /* 0x124 */
    unsigned char  permanent;                     /* 0x125 */
    unsigned char  disallow_blocks;               /* 0x126 */
    unsigned char  disallow_sets;                 /* 0x127 */
    unsigned char  variables_predetermined;       /* 0x128 */
} hs_compile_globals_t;

#ifdef __cplusplus
extern "C" {
#endif

extern hs_compile_globals_t hs_compile_globals;

#ifdef __cplusplus
}
#endif
