#include "../../headers/vidDRIVER.h"
#include "../../headers/ws/vid/vidLOCK.h"

// boundary -- the video-driver critical-section singleton.
extern vidLOCK vidLock;

// 0x82667B10 -- ?BindToProcessor@vidDRIVER@@...
// Record the hardware processor this driver's render thread is pinned to, and bind the video
// lock's underlying platform primitive to the same processor.
void vidDRIVER::BindToProcessor(int threadProcessorNmb)
{
    this->bindProcessor = threadProcessorNmb;
    vidLock.lock.Bind(threadProcessorNmb);
}
