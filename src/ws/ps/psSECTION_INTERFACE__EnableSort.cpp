/* ?EnableSort@psSECTION_INTERFACE@@QAAXH@Z @0x825169C8 */
#include "psSECTION_INTERFACE.h"
#include "psSECTION_DATA.h"
// 0x825169C8 — set (bit 3, 0x08) or clear (bit 2, 0x04) the record table's sort-dirty flag within
// `state`'s low nibble (bits 0/1 are preserved, untouched by either case).

void psSECTION_INTERFACE::EnableSort(int enable)
{
    psSECTION_DATA *data = this->pData;
    if (data) {
        unsigned char base = data->state & ~0x0Cu; /* bits unnamed in DB */
        data->state = enable ? (base | 0x08) : (base | 0x04);
    }
}
