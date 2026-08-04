#include "../headers/hcex/ssl_exporter/SSLExportEntry.h"
#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

// dsVECTOR<SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry, 8>::operator[] (non-const) @ 0x823C8B78
// Bounds-checked mutable access into the SSL export table.
template<>
SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry &
dsVECTOR<SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry, 8>::operator[](int idx)
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            567,
            "idx", idx,
            "nElem", this->nElem);
    return this->pData[idx];
}
