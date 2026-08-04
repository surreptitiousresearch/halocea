#include "headers/ws/vid/vidDRIVER_screenshot.h"

// 0x82667DB4
void vidDRIVER::QueueScreenshot(pctPICTURE *pic)
{
    this->pQueuedScreenshot = pic;
    this->isQueuedScreenReady = 0;
}
