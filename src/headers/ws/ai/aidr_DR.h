#pragma once

// ws-engine ai (aidr = "AI deferred request"): base class for a deferred collision/detection
// request that is produced one frame and analysed the next. DB-verified layouts.
namespace aidr {

// DB-verified values (types_enum_values aidr::DR_TYPE).
enum DR_TYPE {
    DR_VISIBILITY  = 0,
    DR_FIND_ENEMY  = 1,
    DR_TRAJECTORY  = 2,
    DR_REACHABILITY = 3,
    DR_FLOOR_PLACE = 4,
    DR_DNS_PNT     = 5,
};

struct DR;

// DB-verified layout (types_members aidr::DR_vtbl): dtr@0, ProduceShootRays@4, AnalyzeResults@8,
// SR_ForgetAllRes@12.
struct DR_vtbl {
    void (__fastcall *dtr_aidr_DR)(DR *self, int deleteFlag); // 0x00 (scalar-deleting dtr; deleteFlag bit0 => free)
    void (__fastcall *ProduceShootRays)(DR *self);  // 0x04
    bool (__fastcall *AnalyzeResults)(DR *self);    // 0x08
    void (__fastcall *SR_ForgetAllRes)(DR *self);   // 0x0C
};

// DB-verified layout (types_members aidr::DR): __vftable@0, typeReq@4, haveResult@8, destroyTimer@12.
struct DR {
    DR_vtbl *__vftable;   // 0x00
    DR_TYPE  typeReq;     // 0x04
    bool     haveResult;  // 0x08
    float    destroyTimer; // 0x0C time-to-live; when it decays to ~0 the request is destroyed
};

} // namespace aidr
