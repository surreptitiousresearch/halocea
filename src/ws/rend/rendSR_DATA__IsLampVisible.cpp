#include "../../headers/ws/rend/rendSR_DATA.h"
#include "../../headers/ws/rend/rendSR_LAMP_DATA.h"
#include "../../headers/ws/lgt/lgtID.h"

// rendSR_DATA::IsLampVisible @ 0x829ED060
// Look up the lamp identified by `lampID` in this record's lamp table. If present, mark it
// "seen this frame" (bit 27, 0x08000000) and return its cached visibility result (bit 24,
// 0x01000000). If absent, append a fresh slot (encoding typeId in bits 16-23, id in bits 0-15,
// plus the initial 0x28000000 flag bits) and report the lamp as not-yet-visible.
//
// Slot packing (rendSR_LAMP_DATA::data): id = bits 0-15, typeId = bits 16-23. The comparison
// mirrors the disasm (lhz/lbz on the lgtID at offsets 2/0), which is endianness-independent.

int rendSR_DATA::IsLampVisible(const lgtID &lampID)
{
    for (int i = 0; i < lampsData.nElem; ++i) {
        unsigned int data = lampsData[i].data;
        if ((short)(data & 0xFFFF) == lampID.id
            && (unsigned char)((data >> 16) & 0xFF) == lampID.typeId) {
            lampsData[i].data |= 0x08000000u;          // mark "seen this frame"
            return (lampsData[i].data >> 24) & 1;      // cached visibility bit
        }
    }

    rendSR_LAMP_DATA entry;
    entry.data = ((unsigned int)lampID.typeId << 16)
               | (unsigned int)(unsigned short)lampID.id
               | 0x28000000u;
    lampsData.PushBack(entry);
    return 0;
}
