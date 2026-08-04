#include "../fio/osASYNC_INFO.h" // NOTE: canonical osASYNC_INFO header already lives under ws/fio — reused, not duplicated

// os subsystem boundary — cancels/releases the completion signal held by a pending async-I/O request.
extern "C" void osDestroySignal(osHANDLE_DUMMY *signal); // boundary — os subsystem

void osASYNC_INFO::Term()
{
    if (holdSignal)
    {
        osDestroySignal(signal);
        signal = nullptr;
        holdSignal = 0;
    }
}
