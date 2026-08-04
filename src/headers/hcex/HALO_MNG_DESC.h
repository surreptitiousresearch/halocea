#pragma once
#include "../ws/ia/iaDESC.h"
// HALO_MNG_DESC — HALO_MNG's descriptor/factory type (the dscDESC-family object that
// dscMAKE_FAMILY/dscRTTI_FAMILY use to construct/brand HALO_MNG instances). Adds no data members
// of its own. DB-verified layout (types_members HALO_MNG_DESC): iaDESC (base)@0, size 164 --
// identical to the base.

struct HALO_MNG_DESC : iaDESC {
    // 0x823E54A4 — default: base-constructs iaDESC; vtable installation implicit.
    HALO_MNG_DESC();

    // 0x823E5518 — releases the same owned sub-objects the base iaDESC destructor does
    // (spSndData, smlDesc's name buffer, smlProg, psCustomSection, propContainerDesc, sslClass)
    // before chaining to ~dscDESC. HALO_MNG_DESC itself owns nothing extra.
    virtual ~HALO_MNG_DESC();

    // 0x823E6C04 — copy-assign: forwards entirely to iaDESC::operator=.
    HALO_MNG_DESC &operator=(const HALO_MNG_DESC &that);
};
