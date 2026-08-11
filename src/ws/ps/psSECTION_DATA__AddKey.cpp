#include "psSECTION_DATA.h"
#include "psSECTION_INTERFACE.h"
#include "psSECTION.h"
#include "psCOMPLEX_KEY_DATA.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsSTRID.h"
#include "../ds/ds_boundary.h"
#include "../ds/ds_assert_boundary.h"
// 0x8251C5C0 — install `obj` under the parsed `complexKey`. A single-element chain adds a record on
// this section directly. A multi-element chain descends: it reuses the existing leaf-key
// sub-section (if the value there is a section), or removes a type-mismatched record and creates a
// fresh empty sub-section, then recurses into that sub-section with the leaf key popped. Returns the
// leaf record index (or 1/0 success for the single-key case) or -1. Consumes the by-value `obj`.

// 0x8200155A — interned empty-string literal used to build an empty dsSTRID. boundary.
extern const char psEmptyStringLiteral[];

int psSECTION_DATA::AddKey(dsDATA obj, unsigned int flags, int lineNmb, psCOMPLEX_KEY_DATA &complexKey)
{
    if (complexKey._psKeysArr.nElem == 0) {
        dsSTRID emptyKey(psEmptyStringLiteral, 1);
        complexKey._psKeysArr.PushBack(emptyKey);
    }

    int result;
    bool objAlreadyEmpty; // tracks (obj.type == nullptr): when true, no teardown needed

    if (complexKey._psKeysArr.nElem == 1) {
        dsDATA value;
        value.type = nullptr;
        value.StoreValue(obj);

        if (!IGNORE_STRONG_ASSERT && complexKey._psKeysArr.nElem <= 0)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "idx >= 0 && idx < nElem",
                "D:\\Projects\\code\\common\\src.sys\\ps\\ps_section.cpp",
                26,
                empty_string);

        int leaf = this->AddRec(complexKey._psKeysArr.pData[0], value, flags, lineNmb);
        objAlreadyEmpty = (obj.type == nullptr);
        result = (leaf != -1);
    } else {
        dsSTRID lastKey = complexKey._psKeysArr.Back();
        int recIdx = this->FindRecThisOnly(lastKey);
        if (recIdx != -1) {
            dsDATA existingValue = this->GetKeyValue(recIdx);
            bool typeMismatch = (existingValue.type != dsDATA_TYPE_STORAGE<psSECTION>::dataType);
            if (existingValue.type)
                existingValue.type->Destroy(&existingValue.storage);
            if (typeMismatch) {
                this->RemoveRec(recIdx);
                recIdx = -1;
            }
        }

        psSECTION subSection;
        subSection.pData = nullptr;
        if (recIdx == -1) {
            // No usable sub-section: create a fresh empty one and record it under the leaf key.
            ((psSECTION_INTERFACE &)subSection).CreateEmpty();
            dsDATA boxedSub;
            boxedSub.type = nullptr;
            if (subSection.pData)
                boxedSub.StoreValue<psSECTION>(subSection);
            this->AddRec(complexKey._psKeysArr.Back(), boxedSub, flags, lineNmb);
        } else {
            // Descend into the existing sub-section value.
            dsDATA existingValue = this->GetKeyValue(recIdx);
            psSECTION extracted(existingValue);
            subSection = extracted;
            if (extracted.pData)
                extracted.pData->Release();
            if (existingValue.type)
                existingValue.type->Destroy(&existingValue.storage);
        }

        // Pop the leaf key, then recurse into the sub-section with the shortened chain.
        if (complexKey._psKeysArr.nElem > 0)
            --complexKey._psKeysArr.nElem;

        dsDATA value;
        value.type = nullptr;
        value.StoreValue(obj);
        result = subSection.pData->AddKey(value, flags, lineNmb, complexKey);
        if (subSection.pData)
            subSection.pData->Release();
        objAlreadyEmpty = (obj.type == nullptr);
    }

    if (!objAlreadyEmpty)
        obj.type->Destroy(&obj.storage);
    return result;
}
