/* HCEX_OBJ_CMP::cmp(const HCEX_OBJ&, int) @0x823ED700 — same ascending 3-way compare, against a
 * raw id key (used by dsVECTOR<HCEX_OBJ,8>::FindSorted's key search).
 *
 * DEVIATION: same overflow-safe-XOR-compare-to-plain-signed-compare simplification as
 * HCEX_OBJ_CMP__cmp_obj.cpp; verified equivalent. */

#include "../../headers/hcex/HCEX_OBJ_CMP.h"

int HCEX_OBJ_CMP::cmp(const HCEX_OBJ &a, int b) const
{
    if ( a.id < b )
        return -1;
    if ( a.id > b )
        return 1;
    return 0;
}
