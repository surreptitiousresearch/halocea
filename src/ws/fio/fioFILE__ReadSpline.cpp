#include "fioFILE.h"
#include "../m3d/m3dSPL.h"

// fioFILE::ReadSpline() @ 0x825ADF58 — read one spline. A leading 4-byte tag word
// (read byte-reordered in 4-byte groups) selects the format: 0xFFFFFFFF (-1) means
// the current m3dSPL::Read layout; any other value is a legacy count consumed by
// m3dSPL::ReadOld as its first field.

m3dSPL *fioFILE::ReadSpline()
{
    int tag[4];
    ReadData((char *)tag, 4, 4);

    if ( tag[0] == -1 )
        return m3dSPL::Read(this);
    return m3dSPL::ReadOld(tag[0], this);
}
