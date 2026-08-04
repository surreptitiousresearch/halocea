#include "dsSTATIC_PARAM_LIST.h"
#include "dsTSTRING.h"

// dsSTATIC_PARAM_LIST<1>::Add<dsTSTRING<char>>(const char*, const dsTSTRING<char>&) @ 0x823D7B10
// Append a string-valued parameter into the next free inline slot (storage[length]), interning the
// name `id` into a dsSTRID first, then bump length. The slot's value was left empty by the
// constructor, so this is a plain SetValue with no prior Destroy.
template<>
template<>
void dsSTATIC_PARAM_LIST<1>::Add<dsTSTRING<char>>(const char *id, const dsTSTRING<char> &val)
{
    dsSTRID name(id, false);        // dsSTRID::dsSTRID(const char*, bool) — intern the name

    dsPARAM &slot = this->storage[this->length];
    slot.id.id = name.id;
    slot.data.SetValue<dsTSTRING<char>>(&val);
    ++this->length;
}
