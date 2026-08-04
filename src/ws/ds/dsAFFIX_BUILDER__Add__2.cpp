#include "dsAFFIX_BUILDER.h"

// dsAFFIX_BUILDER::Add @ 0x82A786C0
// Return the id of the affix named `name`, appending a fresh (empty) affix with that name if
// none exists yet. A blank affix is default-constructed, copied into the vector, then its name
// is assigned in place (the temporary's destructor — params vector teardown plus name-buffer
// release — is emitted inline in the original).
int dsAFFIX_BUILDER::Add(const dsTSTRING<char> &name)
{
    int existingId = this->FindAffix(name);
    if (existingId != -1)
        return existingId;

    AFFIX newAffix;
    this->affixes.PushBack(newAffix);
    AFFIX &appended = this->affixes.Back();
    appended.name = name;
    return this->affixes.nElem - 1;
}
