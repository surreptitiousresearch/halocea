#include "m3dV.h"
#include "m3d_boundary.h"

// m3dV::operator+= @ 0x823CD3C8 (??Ym3dV@@QAAAAU0@ABU0@@Z)
// Component-wise add-assign of `rhs` into *this, returning *this. The two debug finiteness
// checks (_m3dCheckValid) are preserved; their int result is discarded.
m3dV &m3dV::operator+=(const m3dV &rhs)
{
    _m3dCheckValid(&rhs);
    _m3dCheckValid(this);

    this->x = rhs.x + this->x;
    this->y = rhs.y + this->y;
    this->z = rhs.z + this->z;
    return *this;
}
