#pragma once
/* hs_tokenizer — HaloScript tokenizer state. Database type has a single field; the array form used at call
 * sites (hs_tokenizer state[24]) is just stack scratch space the tokenizer writes through `cursor`. */

typedef struct hs_tokenizer
{
    char *cursor;   /* 0x00 — current parse position in the source buffer */
} hs_tokenizer;
