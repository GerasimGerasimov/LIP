#pragma once

typedef
union bauint
{ // Byte-addressable UINT
    u16 i; // int: Var.i
    s16 s; // int: Var.i
    u8 b[2]; // u char: Var.b[0]:Var.b[1]
} bauint;

typedef
union baulong
{ // Byte-addressable UINT
    u32 L;    // 1 DWORD
    float f;
    u16 i[2]; // 2 WORD
    u8 b[4]; // 4 BYTES
} baulong;