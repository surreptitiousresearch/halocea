#pragma once
#include <stdint.h>
/* search_and_replace_reference — one element of ui_widget_definition.search_and_replace_references
   (34 bytes; DB-verified). */

typedef struct search_and_replace_reference
{
    char    search_string[32];  /* 0x00 */
    int16_t replace_function;   /* 0x20 */
} search_and_replace_reference; /* 34 bytes */
