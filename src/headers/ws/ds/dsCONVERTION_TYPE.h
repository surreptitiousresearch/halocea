#pragma once
// ws-engine ds_data: conversion direction/kind passed to dsDATA_TYPE conversions.
// DB-verified (types_enum_values dsCONVERTION_TYPE) — size 4.

enum dsCONVERTION_TYPE {
    DSD_CONV_STORE    = 0,
    DSD_CONV_RETRIEVE = 1,
    DSD_CONV_EXPLICIT = 2,
    DSD_CONV_PARAM    = 3,
    DSD_CONV_MATH     = 4,
};
