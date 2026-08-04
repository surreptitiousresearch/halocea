#pragma once
/* HaloScript global designator: bit 15 selects the pool (1 = engine-registered external
 * global in hs_external_globals[], 0 = scenario-defined global in the scenario's hs_globals
 * block), low 15 bits are the index into the selected pool.
 *
 * Convention header (same reconstruction basis as datum_index.h / BIT_VECTOR_*): the names
 * are derived, not recovered — the DB carries no enum for these values. Basis:
 * hs_find_global_by_name returns (i | 0x8000) for externals and every consumer tests bit 15
 * then masks with 0x7FFF. Raw -1/0xFFFF sentinel COMPARES stay raw (locked convention). */

#define HS_GLOBAL_EXTERNAL_FLAG                    0x8000
#define HS_GLOBAL_IS_EXTERNAL(designator)          (((designator) & HS_GLOBAL_EXTERNAL_FLAG) != 0)
#define HS_GLOBAL_DESIGNATOR_TO_INDEX(designator)  ((designator) & 0x7FFF)
#define MAKE_EXTERNAL_GLOBAL_DESIGNATOR(index)     ((index) | HS_GLOBAL_EXTERNAL_FLAG)
