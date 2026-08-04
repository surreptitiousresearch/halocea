#pragma once
#include "hkArray.h"

/* Havok filesystem boundary (opaque sink + string/array helpers). */
typedef struct hkFileSystem_DirectoryListing hkFileSystem_DirectoryListing; /* hkFileSystem::DirectoryListing */

void hkFileSystem_DirectoryListing_addDirectory(hkFileSystem_DirectoryListing *self, const char *name);
void hkFileSystem_DirectoryListing_addFile(hkFileSystem_DirectoryListing *self, const char *name, unsigned long long writeTime);

void hkNativeFileSystem_s_havokToPlatformConvert(const char *havokPath, hkArray<char> *platformPathOut);

int hkString_strLen(const char *s);                       /* hkString::strLen */
void hkString_memCpy(void *dst, const void *src, int n);  /* hkString::memCpy */
int hkString_strCmp(const char *a, const char *b);        /* hkString::strCmp */

void hkArrayUtil__reserve(void *array, int count, int elementSize);   /* hkArrayUtil::_reserve */
void hkArrayUtil__reserveMore(void *array, int elementSize);          /* hkArrayUtil::_reserveMore */
