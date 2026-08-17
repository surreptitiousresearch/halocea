/* ?IsEnabled@UI_FRAME@ui_new@@QAA_NXZ @0x827D31A8 */
#include "../../headers/ws/ui_new/UI_FRAME.h"

// 0x827D31AC — return whether this frame is currently enabled.
// Typed member read (types_members ui_new::UI_FRAME::isEnabled @0x11; UI_FRAME.h has it named).
bool ui_new::UI_FRAME::IsEnabled()
{
    return this->isEnabled;
}
