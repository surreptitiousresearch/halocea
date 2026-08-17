/* ?Alloc@objSPLIT@@QAAHH@Z @0x82BC9E20 */
#include "objSPLIT.h"
#include "obj_boundary.h"
// 0x82BC9E20  ?Alloc@objSPLIT@@QAAHH@Z
// Resize the split's entry array to `nEntry` entries. When shrinking, entries above the new count are
// reset and their material tables released before reallocation; when growing, freshly exposed entries
// are default-initialized (empty material table, no spuConfig, invalid shader lengths). Returns 1 on
// success (also when nEntry==0 and the array frees to null), 0 on reallocation failure.

int objSPLIT::Alloc(int nEntry)
{
    if (nEntry < this->nEntry) {
        for (int i = nEntry; i < this->nEntry; ++i) {
            objSPLIT_ENTRY &entry = this->entryList[i];
            entry.startVert = 0;
            entry.nVert = 0;
            entry.startFace = 0;
            entry.nFace = 0;
            entry.mtlTbl.Clear();
            entry.skinCompoundId = -1;
            entry.bonesInfo.start1 = 0;
            entry.bonesInfo.start2 = 0;
            entry.bonesInfo.num1 = 0;
            entry.bonesInfo.num2 = 0;
            entry.maxPSLength = -1;
            entry.maxVSLength = -1;
            // STRONG_ASSERT: entryList[i].spuConfig == 0  (obj_split.cpp:607)
        }
    }

    objSPLIT_ENTRY *newList = (objSPLIT_ENTRY *)dlRealloc(
        this->entryList, sizeof(objSPLIT_ENTRY) * nEntry,
        "D:\\Projects\\code\\common\\src.sys\\objects\\obj_split.cpp", 0x263);
    this->entryList = newList;
    if (!newList)
        return nEntry == 0;

    int oldEntry = this->nEntry;
    if (oldEntry < nEntry) {
        for (int i = oldEntry; i < nEntry; ++i) {
            objSPLIT_ENTRY &entry = this->entryList[i];
            entry.startVert = 0;
            entry.nVert = 0;
            entry.startFace = 0;
            entry.nFace = 0;
            // construct the material vector (inlined mtlTBL ctor), then initialize the table
            entry.mtlTbl.mtlList.pData = nullptr;
            entry.mtlTbl.mtlList.nElem = 0;
            entry.mtlTbl.mtlList.allocated = 0;
            entry.mtlTbl.mtlList.__cl.file = "D:\\Projects\\code\\common\\INCL.SYS\\material.h";
            entry.mtlTbl.mtlList.__cl.line = 210;
            entry.mtlTbl.Init();
            entry.spuConfig = nullptr;
            entry.skinCompoundId = -1;
            entry.bonesInfo.start1 = 0;
            entry.bonesInfo.start2 = 0;
            entry.bonesInfo.num1 = 0;
            entry.bonesInfo.num2 = 0;
            entry.maxVSLength = -1;
            entry.maxPSLength = -1;
        }
    }

    this->nEntry = nEntry;
    return 1;
}
