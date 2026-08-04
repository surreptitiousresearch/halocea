#include "dsSTATIC_PARAM_LIST.h"
#include "dsTSTRING.h"

// dsSTATIC_PARAM_LIST<2>::Add<dsTSTRING<char>>(dsSTRID, const dsTSTRING<char>&) @ 0x823D84D8
// Append a string-valued parameter named `id` into the next free inline slot (storage[length]),
// then bump length. The slot's value was left empty by the constructor, so this is a plain
// SetValue with no prior Destroy.
template<>
template<>
void dsSTATIC_PARAM_LIST<2>::Add<dsTSTRING<char>>(dsSTRID id, const dsTSTRING<char> &val)
{
    dsPARAM &slot = this->storage[this->length];
    slot.id.id = id.id;
    slot.data.SetValue<dsTSTRING<char>>(&val);
    ++this->length;
}
