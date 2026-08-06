#pragma once
// DB-verified via types_members _303708F16CCD5B5D084E785631C2CA68 (anonymous PDB type)
/* hs_compile_globals — HaloScript compiler state. Layout taken verbatim from the database anonymous struct
 * type (_303708F16CCD5B5D084E785631C2CA68); field names and offsets are authoritative. */

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
