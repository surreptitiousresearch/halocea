#pragma once

struct fioFILE; // ws-engine fio — forward (boundary)

namespace ds_data {

// Per-T disk (fioFILE) serialization policy. For types that opt out of file streaming both
// statics are stubs that trip a strong-assert and return 0 (see the reversed specializations,
// e.g. dsDATA_TYPE_STREAM<haloRAW_INPUT_X360_ENV>::Save/Load, DB 0x823BCC10 / 0x823BCC70).
template<class T>
struct dsDATA_TYPE_STREAM {
    static int Save(const T &data, fioFILE *pFile);
    static int Load(fioFILE *pFile, T &data);
};

} // namespace ds_data
