/* HCEX_OBJ_CMP::cmp(const HCEX_OBJ&, const HCEX_OBJ&) @0x823ED73C — ascending 3-way compare by id.
 *
 * DEVIATION: the decompiler renders this as an overflow-safe XOR-based compare:
 *   if (a.id >= b.id) return (((a.id ^ b.id) >= 0) + ((unsigned)b.id >= (unsigned)a.id)) & 1;
 *   else               return -1;
 * Verified equivalent (by truth table over the same-sign / cross-sign / equal cases) to a plain
 * signed 3-way compare on `id`. Reproduced in the simpler equivalent form. */

#include "../../headers/hcex/HCEX_OBJ_CMP.h"

int HCEX_OBJ_CMP::cmp(const HCEX_OBJ &a, const HCEX_OBJ &b) const
{
    if ( a.id < b.id )
        return -1;
    if ( a.id > b.id )
        return 1;
    return 0;
}
