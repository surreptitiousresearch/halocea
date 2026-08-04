#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::GetRawInput @ 0x823C23C0 — fetch the raw input last recorded for `userId`.
void haloENGINE_CONTROL::GetRawInput(int userId, haloRAW_INPUT_X360 *inp)
{
    hcex_raw_input_get(userId, inp);
}
