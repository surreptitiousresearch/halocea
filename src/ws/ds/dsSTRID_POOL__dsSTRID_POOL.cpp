#include "dsSTRID_POOL.h"
#include "ds_boundary.h"
#include "ds_assert_boundary.h"

// dsSTRID_POOL::dsSTRID_POOL @ 0x8252B000
// Default-construct the pool: init the sorted map (mem-initializer, registering it with its
// erase manager), zero the string-block bookkeeping (idxStorage/posStorage/strStorage) — the
// pool's own chunk table is left untouched here (heap memory from the caller's `operator new`
// is assumed already zeroed, per the disassembly not re-zeroing it) — then presize the map's
// hash table to the next prime >= 4096 buckets, all null.
dsSTRID_POOL::dsSTRID_POOL()
    : stringMap(apCL{"D:\\Projects\\code\\common\\src.sys\\ds\\ds_strid.cpp", 0x133})
{
    this->stringTable.numChunks  = 0;
    this->stringTable.emptyChain = -1;
    this->stringTable.cl = apCL{"D:\\Projects\\code\\common\\src.sys\\ds\\ds_strid.cpp", 0x134};
    this->_cl.file = "D:\\Projects\\code\\common\\src.sys\\ds\\ds_strid.cpp";
    this->_cl.line = 0x135;

    dsERASE_MANAGER<dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::LINE_ERASER> *eraseMgr =
        dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::GetEraseMgr();
    eraseMgr->RegisterThread();

    this->idxStorage = 0;
    this->posStorage = 0;
    for (int slot = 0; slot < 256; ++slot)
        this->strStorage[slot] = nullptr;

    int hashTableSize = ds::FindPrime(0x1000);
    this->stringMap.hashTable.Resize(hashTableSize);

    int bucketCount = this->stringMap.hashTable.nElem;
    for (int idx = 0; idx < bucketCount; ++idx) {
        if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->stringMap.hashTable.nElem))
            STRONG_ASSERT2_HELPER::asserd(
                "idx >= 0 && idx < nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                0x237,
                "idx", idx,
                "nElem", this->stringMap.hashTable.nElem);
        this->stringMap.hashTable.pData[idx] = nullptr;
        bucketCount = this->stringMap.hashTable.nElem; // re-read live, matching the disasm's reload each iteration
    }
}
