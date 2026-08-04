#ifndef BANK_ITEM_S_H
#define BANK_ITEM_S_H

/* bank_item_s — one row of a preview_list dynamic_array bank (preview_list_array_bank[3]).
 * DB-verified layout (types_members), 16 bytes. */
typedef struct bank_item_s
{
    unsigned __int16 *name;       /* 0x00 — display string (dlMalloc'd) */
    void             *ptr;        /* 0x04 — associated data payload (dlMalloc'd) */
    int               list_index; /* 0x08 */
    unsigned __int8   is_default; /* 0x0C */
} bank_item_s;                    /* 16 bytes */

#endif /* BANK_ITEM_S_H */
