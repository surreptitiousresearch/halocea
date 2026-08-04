#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;              // _IGNORE_STRONG_ASSERT
extern unsigned char STRONG_ASSERT_INFO_STUB; // byte_8200155A

// aiCOORDINATOR::GetGroup (const, by brain) @ 0x832352D8
// ?GetGroup@aiCOORDINATOR@@UBAPBVaiCOORD_GROUP@@PAVaiBRAIN@@@Z
//
// const overload of GetGroup(aiBRAIN*): returns the group cached on the unit's status, with the
// same debug consistency assert against the membership scan (asserts at a08_coord.cpp:402).
const aiCOORD_GROUP *aiCOORDINATOR::GetGroup(aiBRAIN *pBrain)
{
    if (!pBrain)
        return nullptr;

    aiCOORD_GROUP *cached = pBrain->st.pointee->GetCoordGroup();

    aiCOORD_GROUP *found = nullptr;
    int nElem = this->groups.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        if (this->groups[i]->IsContain(reinterpret_cast<iaIACTOR *>(pBrain)))
        {
            found = this->groups[i];
            break;
        }
    }

    if (!IGNORE_STRONG_ASSERT && cached != found)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "gpr == gpr2",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_coord.cpp", 402,
            STRONG_ASSERT_INFO_STUB);

    return cached;
}
