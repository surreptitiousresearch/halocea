/* ?FlushScreenshotQueue@vidDRIVER@@QAAXPAVtxmTEXTURE@@@Z @0x82667DC8 */
#include "headers/ws/vid/vidDRIVER_screenshot.h"

// 0x82667DD8
void vidDRIVER::FlushScreenshotQueue(txmTEXTURE *backbuf)
{
    if (this->pQueuedScreenshot && !this->isQueuedScreenReady)
        this->isQueuedScreenReady = this->ReadBackBuffer(this->pQueuedScreenshot, backbuf);
}
