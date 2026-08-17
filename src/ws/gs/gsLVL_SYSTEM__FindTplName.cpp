/* ?FindTplName@gsLVL_SYSTEM@@UAAPAVanimTPL@@PBDPAVdsAFFIX_STRING@@H@Z @0x827986D0 */
#include "../../headers/ws/gs/gsLVL_SYSTEM.h"
#include "../../headers/ws/gs/gsLVL_SYSTEM_boundary.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// 0x827986D0 — resolve an animation template by name (vtable slot 0x30).
//   - Empty name -> the scene's dummy template.
//   - Otherwise strip a trailing ".tpl" and look the name up in the resident template store.
//   - On a hit that is the dummy sentinel, return it as-is.
//   - On a miss (null): fail if `noLoad`, else allocate a slot and stream the template in,
//     running the start/finish load callbacks.
//   - Finally, bind materials/affix (_FinishTplLoadMtl); destroy and fail if that step fails.
animTPL *gsLVL_SYSTEM::FindTplName(const char *nameTpl, dsAFFIX_STRING *instAff, int noLoad)
{
    animTPL *pTpl;

    if (nameTpl[0] == '\0') {
        // Empty name: the scene's dummy template stands in.
        pTpl = gsScenePtr->pTplDummy;
    } else {
        dsTSTRING<char> name;
        name.pBuffer = nullptr;
        name.UnsafeInit(nameTpl, -1, 0);

        // Drop a trailing ".tpl" extension if present.
        int extPos = name.RFind(".tpl", -1, 0);
        if (extPos >= 0 && extPos == name.pBuffer->strLen - 4)
            name.Erase(name.pBuffer->strLen - 4, -1);

        pTpl = gsScenePtr->FindTplName(name.pBuffer->str);

        // Manual release of the scratch string buffer (matches the inlined dsTSTRING destructor).
        dsTSTRING_BUF_HEADER<char> *buf = name.pBuffer;
        if (--buf->refCount == 0)
            dlFree(buf);
    }

    // A dummy-template hit is returned verbatim.
    if (gsScenePtr->pTplDummy == pTpl)
        return pTpl;

    if (!pTpl) {
        // Not resident — load on demand unless suppressed.
        if (noLoad)
            return nullptr;

        pTpl = gsScenePtr->AllocTpl();
        if (!pTpl) {
            _apLog("~Level,Error~Cannot create template");
            return nullptr;
        }

        TplStartLoadCB(pTpl, this);

        if (!LoadTpl(nameTpl, pTpl, 0)) {
            sioSetObjLoadCB(ObjLoadCB);
            gsScenePtr->DestroyTpl(pTpl);
            _apLog("~Level,Error~Cannot load template %s", nameTpl);
            return nullptr;
        }

        if (!TplFinishLoadCB(pTpl, this)) {
            _apLog("~Level,Error~Cannot load template %s", nameTpl);
            return nullptr;
        }
    }

    // Bind materials / affix overrides; on failure, drop the template.
    if (_FinishTplLoadMtl(pTpl, instAff))
        return pTpl;

    gsScenePtr->DestroyTpl(pTpl);
    return nullptr;
}
