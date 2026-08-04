#include "sslERROR.h"

// sslERROR::operator=(const sslERROR &other) @ 0x823C4F38
// Field-wise copy-assign: id and tags are plain scalars; desc goes through dsTSTRING::operator=
// (ref-counted share/release, boundary).
sslERROR &sslERROR::operator=(const sslERROR &other)
{
    this->id = other.id;
    this->desc = other.desc;
    this->tags = other.tags;
    return *this;
}
