// scrSCORCH_MANAGER::UnfarmAll @ 0x827274D0
// Arms the "un-farm every static scorch mark" request by setting the unfarm threshold to a value
// larger than any real farmed-mark count, so the next scorch update pulls all farmed marks back.
#include "../../headers/ws/scorch/scrSCORCH_MANAGER.h"

void scrSCORCH_MANAGER::UnfarmAll()
{
    this->nScorchStatToUnfarm = 1000;
}
