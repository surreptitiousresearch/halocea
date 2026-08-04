// aiNAV_WP_WRAPPER_TPL::ChangeOrAdd @ 0x8321A668
#include "../../headers/ws/ai/aiNAV_WP_WRAPPER_TPL.h"
#include "../../headers/ws/ai/aiNAV_WP_DATA.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h" // struct wpWP (ExtendToTRK/SetParentObject)
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/animINST.h"
#include "../../headers/ws/phys/physSYSTEM_BASE.h"
#include "../../headers/ws/m3d/m3dMATR.h"

extern int IGNORE_STRONG_ASSERT;    // _IGNORE_STRONG_ASSERT
extern char byte_8200155A;          // shared empty-string byte @0x8200155A
extern "C" int sprintf_0(char *string, const char *format, ...);

// TU-local scratch buffer for the name-scoped waypoint key (sizeof == 0x80 in the binary).
static char nameModified[128];

static const char *const kSrcFile =
    "D:\\Projects\\code\\common\\src.sys\\gm_shared\\nav\\nav_file_wrp_tpl.cpp";

int aiNAV_WP_WRAPPER_TPL::ChangeOrAdd(const aiNAV_WP_DATA &data, const char *nameWP)
{
    animINST *pInst = parentEntity->pInst;

    if (!IGNORE_STRONG_ASSERT)
    {
        if (!pInst)
            STRONG_ASSERT_DUMMY::Crash(nullptr, "pInst", kSrcFile, 47, &byte_8200155A);

        const char *instName = pInst->name ? pInst->name : &byte_8200155A;
        // sizeof(nameModified) must exceed strlen(pInst->GetName())+2+strlen(nameWP).
        size_t need = 2u;
        for (const char *p = instName; *p; ++p) ++need;
        for (const char *p = nameWP;  *p; ++p) ++need;
        if (need >= sizeof(nameModified))
            STRONG_ASSERT_DUMMY::Crash(
                nullptr,
                "sizeof(nameModified) > strlen(pInst->GetName())+2+strlen(nameWP)",
                kSrcFile, 51, &byte_8200155A);
    }

    const char *instName = pInst->name ? pInst->name : &byte_8200155A;
    sprintf_0(nameModified, "%s|%s", instName, nameWP);

    short wpID = base.ChangeOrAdd(data, nameModified);
    navWP *wp = base.navManager->GetWP(wpID);
    if (!wp)
        return -1;

    // Promote the payload to a tracked waypoint and parent it to this wrapper's entity.
    wp->wpData = wp->wpData->ExtendToTRK(wp->flags);
    wp->wpData->SetParentObject(parentEntity);
    wp->MakeRelativeDirRefHO();

    if (data.haveMatrix)
    {
        // Waypoint matrix, transformed into the model instance's local space.
        m3dMATR wpMatr = data.matrix;
        m3dMATR l2w;
        physSYSTEM_BASE *phys = pInst->pPhysSystem;
        if (phys && phys->pSystemSafe)
            phys->GetMatrMainBody(l2w);
        else
            pInst->_GetMatrL2W(&l2w);
        wpMatr.Transform(&l2w, 2);
        wp->BindToInst(pInst, &wpMatr);
    }

    if (doAutolink)
    {
        base.navManager->AutolinkWP(wpID);
        base.navManager->AddHOJumpBatch(wp, wpID);
    }

    return wpID;
}
