#pragma once
/* ai_animation_reference_definition — one scenario AI animation reference (60 bytes). Full DB layout. */

#include "tag_reference.h"

typedef struct ai_animation_reference_definition
{
    char          animation_name[32];  /* 0x00 */
    tag_reference animation_graph;     /* 0x20 — .index at +44 */
    unsigned int  unused[3];           /* 0x30 */
} ai_animation_reference_definition;   /* 60 bytes */
