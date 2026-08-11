/* HCEX_OBJ default ctor @0x823EDA00 — default-construct: no tracked Blam object (id=-1), empty
 * instance pointer, empty followers/followersOfs vectors (adopting the "ds_vector.h":24 allocation
 * call-site cookie for both), and flags cleared.
 *
 * HCEX_OBJ.h keeps HCEX_OBJ a plain C-compatible struct (both .c and .cpp translation units include
 * it), so this ctor is modeled as a free helper rather than a real HCEX_OBJ:: member, matching the
 * convention already used by the dsVECTOR<HCEX_OBJ,8> template-instantiation files.
 *
 * The decompiler shows the flags store as
 * `*this+13 = __ROR4__(1,7) & 0xFFFE0000 | *this+13 & 0x1FFFF` — the low 17 reserved bits are
 * preserved and bits 17..31 are taken from `ROR(1,7) & 0xFFFE0000`. ROR(1,7) = 0x02000000 (bit 25),
 * and 0x02000000 & 0xFFFE0000 = 0x02000000 (bit 25 lies inside the mask), so the store SETS
 * isInited (HCEX_OBJ_FLAG_IS_INITED, bit 25) — it does NOT clear the word. The default ctor marks
 * the fresh tracking record as inited, matching hcex_obj_init's use of the same bit. */

#include "../../headers/hcex/HCEX_OBJ.h"

void hcex_obj_construct(HCEX_OBJ *obj)
{
    obj->id = -1;
    obj->pInst = 0;
    // modelIdx: DEVIATION — the binary does not write this field in the default ctor at all
    // (verified against the disassembly store list); left uninitialized here to match, rather
    // than inventing a decompiler-omitted store.

    apCL cl;
    cl.line = 24;
    cl.file = "D:\\Projects\\code\\common\\INCL.SYS\\ds\\ds_vector.h";

    obj->followers.pData = 0;
    obj->followers.nElem = 0;
    obj->followers.allocated = 0;
    obj->followers.__cl = cl;

    obj->followersOfs.pData = 0;
    obj->followersOfs.nElem = 0;
    obj->followersOfs.allocated = 0;
    obj->followersOfs.__cl = cl;

    // Preserve the low 17 reserved bits, set isInited (bit 25) — see the header note above.
    HCEX_OBJ_FLAGS(obj) = (HCEX_OBJ_FLAGS(obj) & 0x1FFFFu) | HCEX_OBJ_FLAG_IS_INITED;
}
