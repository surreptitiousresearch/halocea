#pragma once
/* Win32 file-enumeration boundary (platform APIs), reached only by src/havok/hkWin32ListDirectory.
   The two records are PLATFORM boundary types: declared once, in the canonical headers below,
   never re-declared here. This file carried its own copies of both until 2026-08-07, which is an
   ODR redefinition — clang leaves the dependent record incomplete in header_layout's bulk probe,
   and the type then drops out of header_db, header_members and enum_values at once. */

#include "../_FILETIME.h"
#include "../WIN32_FIND_DATAA.h"

unsigned int GetFileAttributesA(const char *fileName);
void *FindFirstFileA(const char *fileName, _WIN32_FIND_DATAA *findData);
int FindNextFileA(void *findHandle, _WIN32_FIND_DATAA *findData);
int CloseHandle(void *object);
