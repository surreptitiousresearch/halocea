/* ?Merge1stLevel@cdtOFL@@QAAXAAV1@@Z @0x827E4DA8 */
#include "../../headers/ws/cdt/cdtOFL.h"

// 0x827E4DA8  ?Merge1stLevel@cdtOFL@@QAAXAAV1@@Z
// Append the primary (1st-level) id list of `ofl` onto this list, clamped to this list's
// capacity (nIDMax).
void cdtOFL::Merge1stLevel(cdtOFL &ofl)
{
    int count = ofl.nID;
    if ( this->nID + ofl.nID > this->nIDMax )
        count = this->nIDMax - this->nID;

    for ( int i = 0; i < count; ++i )
        this->idList[this->nID + i] = ofl.idList[i];

    this->nID += count;
}
