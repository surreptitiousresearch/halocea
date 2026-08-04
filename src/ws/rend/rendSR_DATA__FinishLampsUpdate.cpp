#include "../../headers/ws/rend/rendSR_DATA.h"
#include "../../headers/ws/rend/rendSR_LAMP_DATA.h"

// rendSR_DATA::FinishLampsUpdate @ 0x829ECFCC
// End a static-lamp visibility pass: compact the lamp table in place, keeping only the slots
// marked "visible" this frame (bit 27, 0x08000000), then shrink the vector to the surviving
// count. The compaction is a stable in-order copy-down (read index v3, write index writeCount).

void rendSR_DATA::FinishLampsUpdate()
{
    int writeCount = 0;
    for (int i = 0; i < lampsData.nElem; ++i) {
        if ((lampsData[i].data & 0x08000000u) != 0) {
            lampsData[writeCount].data = lampsData[i].data;
            ++writeCount;
        }
    }
    lampsData.Resize(writeCount);
}
