#pragma once

/* prop_iterator — cursor over an actor's prop list, used with prop_iterator_new/prop_iterator_next. 8 bytes. */
typedef struct prop_iterator
{
    int index;        /* 0x0 — current prop index */
    int next_index;   /* 0x4 — next prop index to visit */
} prop_iterator;
