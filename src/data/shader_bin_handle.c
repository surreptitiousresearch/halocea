#include "../headers/chicago_shaders.h"
/* DB size 8 but declared type is void*(4); trailing 4 bytes belong to a following global. */
/* bytes: FFFFFFFF = invalid handle sentinel */
void *shader_bin_handle = (void *)0xFFFFFFFFu;
