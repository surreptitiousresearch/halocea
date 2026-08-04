#include "dsAFFIX_BUILDER.h"
#include "dsTSTRING.h"
#include "dsTSTRING_BUF_HEADER.h"

// dsAFFIX_BUILDER::Add @ 0x82A77760
// Set the parameter named `name` to `value` on affix `affixIndex`. Ignores empty operands; if a
// parameter with that name already exists, overwrites its value in place; otherwise appends a fresh
// {name, value} parameter. (The original inlines the AFF_PARAM copy-assigns and its destructor.)
void dsAFFIX_BUILDER::Add(int affixIndex, const dsTSTRING<char> &name, const dsTSTRING<char> &value)
{
    AFFIX &affix = this->affixes[affixIndex];

    if (name.pBuffer->strLen == 0 || value.pBuffer->strLen == 0)
        return; // ignore an empty name or value

    for (int i = 0; i < affix.params.nElem; ++i) {
        if (affix.params[i].name == name) {
            affix.params[i].value = value; // overwrite existing parameter's value
            return;
        }
    }

    // not present: append a new parameter sharing the name/value buffers
    AFF_PARAM param;
    param.name = name;
    param.value = value;
    affix.params.PushBack(param);
}
