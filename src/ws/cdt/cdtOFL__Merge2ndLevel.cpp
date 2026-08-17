/* ?Merge2ndLevel@cdtOFL@@QAAXAAV1@@Z @0x827E4E10 */
#include "../../headers/ws/cdt/cdtOFL.h"

// 0x827E4E10  ?Merge2ndLevel@cdtOFL@@QAAXAAV1@@Z
// Append the secondary (2nd-level) id list of `ofl` onto this list, clamped to this list's
// secondary capacity (nID2Max).
void cdtOFL::Merge2ndLevel(cdtOFL &ofl)
{
    int count = ofl.nID2;
    if ( this->nID2 + ofl.nID2 > this->nID2Max )
        count = this->nID2Max - this->nID2;

    for ( int i = 0; i < count; ++i )
        this->id2List[this->nID2 + i] = ofl.id2List[i];

    this->nID2 += count;
}
