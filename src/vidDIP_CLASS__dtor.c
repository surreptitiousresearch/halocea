#include "headers/ws/vid/vidDIP.h"

// @0x8266A1B8 -- dipList's ~dsVECTOR<vidDIP,8> (which itself destroys each contained vidDIP,
// releasing their name/dipIDList) runs first, then `name`'s buffer reference is dropped. Both
// happen automatically via member destructors; body intentionally empty (matches the
// decompiler's explicit ~dsVECTOR + refcount-- sequence, just expressed as RAII).
vidDIP_CLASS::~vidDIP_CLASS()
{
}
