#pragma once
#include <stdint.h>
/* hs_function_definition — one entry in hs_function_table (the HaloScript built-in function/operator table).
 * formal_parameters is a variable-length __int16[] tail indexed by formal_parameter_count. */

typedef struct hs_function_definition
{
    int16_t        return_type;                                            /* 0x00 */
    unsigned char  _pad02[2];                                              /* 0x02 */
    char          *name;                                                   /* 0x04 */
    unsigned char  (__fastcall *parse)(int16_t, int);                      /* 0x08 */
    void           (__fastcall *evaluate)(int16_t, int, unsigned char);    /* 0x0C */
    const char    *documentation;                                          /* 0x10 */
    const char    *parameters;                                             /* 0x14 */
    unsigned char  console_flags;                                          /* 0x18 */
    unsigned char  _pad19[1];                                              /* 0x19 */
    int16_t        formal_parameter_count;                                 /* 0x1A */
    int16_t        formal_parameters[];                                    /* 0x1C */
} hs_function_definition;

extern const hs_function_definition *hs_function_table[471]; /* DB applied type: const, [471] */
