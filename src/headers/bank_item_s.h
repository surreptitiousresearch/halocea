#include <stdint.h>
#ifndef BANK_ITEM_S_H
#define BANK_ITEM_S_H

/* bank_item_s — one row of a preview_list dynamic_array bank (preview_list_array_bank[3]).
 * DB-verified layout (types_members), 16 bytes. */
typedef struct bank_item_s
{
    uint16_t *name;       /* 0x00 — display string (dlMalloc'd) */
    void             *ptr;        /* 0x04 — associated data payload (dlMalloc'd) */
    int               list_index; /* 0x08 */
    uint8_t   is_default; /* 0x0C */
} bank_item_s;                    /* 16 bytes */

#endif /* BANK_ITEM_S_H */
