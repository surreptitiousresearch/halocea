#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "ds_assert_boundary.h"

/* CAVEAT: on the type-mismatch path the original folds both Destroy() calls onto one lost
 * ctr-dispatched pointer, so the decompiler does not show which storage each Destroy targets.
 * Reconstructed by RAII semantics: on success destroy THIS's old value and move the converted
 * temp in; on failure destroy the converted temp and keep the old value. */

// dsDATA::SetValue @ 0x823CD570
// Type-preserving copy-set: unlike StoreValue(const dsDATA&) (which adopts src's type), when both
// values are non-empty and of different types this converts src's value into a fresh value of
// THIS's existing type.
void dsDATA::SetValue(const dsDATA &src)
{
    const dsDATA_TYPE *srcType = src.type;
    const dsDATA_TYPE *thisType = this->type;

    if (srcType) {
        if (thisType) {
            if (thisType == srcType) {
                // same type: in-place copy-assign the stored value
                thisType->Copy(&src.storage, &this->storage);
            } else {
                // different type: construct a fresh value of THIS type, convert src into it
                unsigned int converted;
                thisType->Construct(&converted);
                int ok = srcType->Convert(&src.storage, &converted, this->type, DSD_CONV_STORE);
                if (ok) {
                    this->type->Destroy(&this->storage);
                    this->storage = converted;
                } else {
                    this->type->Destroy(&converted);
                    if (!IGNORE_STRONG_ASSERT)
                        STRONG_ASSERT_DUMMY().Crash(
                            "0",
                            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_data_impl.hpp",
                            170,
                            dsStrongAssertMessage);
                }
            }
        } else {
            // this empty: copy-construct from src and adopt src's type
            this->type = srcType;
            srcType->CopyConstruct(&this->storage, &src.storage);
        }
    } else if (thisType) {
        // src empty: drop this value
        thisType->Destroy(&this->storage);
        this->type = nullptr;
    }
}
