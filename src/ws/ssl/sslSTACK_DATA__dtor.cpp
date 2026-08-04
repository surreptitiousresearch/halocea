#include "sslSTACK_DATA.h"

// sslSTACK_DATA::~sslSTACK_DATA() @ 0x82AF0B80
// Thunk straight to Cleanup() (drops any ref-counted payload; DB attribute "thunk").
sslSTACK_DATA::~sslSTACK_DATA()
{
    this->Cleanup();
}
