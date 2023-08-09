// #pragma once

// #include <string>

// #include "../structure/LIR.h"

// static std::string RelOp2String(LIR::RelOp op) {
//     switch (op) {
//     case LIR::RelOp::T_eq: return "eq";
//     case LIR::RelOp::T_ne: return "ne";
//     case LIR::RelOp::T_lt: return "lt";
//     case LIR::RelOp::T_ge: return "ge";
//     case LIR::RelOp::T_gt: return "gt";
//     case LIR::RelOp::T_le: return "le";
//     case LIR::RelOp::F_eq: return "eq";
//     case LIR::RelOp::F_ne: return "ne";
//     case LIR::RelOp::F_lt: return "lt";
//     case LIR::RelOp::F_ge: return "pl";
//     case LIR::RelOp::F_gt: return "hi";
//     case LIR::RelOp::F_le: return "le";
//     default: return "unknown";
//     }
// }

// static std::string BinOp2String(LIR::BIN_OP op) {
//     switch (op) {
//     case LIR::BIN_OP::T_div: return "sdiv";
//     case LIR::BIN_OP::T_minus: return "sub";
//     case LIR::BIN_OP::T_mod: return "mod";
//     case LIR::BIN_OP::T_mul: return "mul";
//     case LIR::BIN_OP::T_plus: return "add";
//     case LIR::BIN_OP::F_div: return "vdiv.f32";
//     case LIR::BIN_OP::F_minus: return "vsub.f32";
//     case LIR::BIN_OP::F_mul: return "vmul.f32";
//     case LIR::BIN_OP::F_plus: return "vadd.f32";
//     default: return "unknown";
//     }
// }