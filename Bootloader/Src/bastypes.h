#ifndef BASTYPES_H
#define BASTYPES_H
#include "stm32f0xx.h"// basic types

typedef u8* pu8;

typedef unsigned short uint;
typedef unsigned char uchar;
typedef unsigned long ulong;

typedef
union bauint
{ // Byte-addressable UINT
  u16 i; // int: Var.i
  u8 b[2]; // u char: Var.b[0]:Var.b[1]
} bauint;


typedef
union bavu16
{ // Byte-addressable UINT
  vu16 i; // int: Var.i
  vu8 b[2]; // u char: Var.b[0]:Var.b[1]
} bavu16;

typedef
union baulong
{ // Byte-addressable UINT
  ulong L;    // 1 DWORD
  uint  i[2]; // 2 WORD
  uchar b[4]; // 4 BYTES
} baulong;

typedef
union bauqword
{ // Byte-addressable QWORD 64bit
  uchar  b[8]; //8  BYTES
  uint   i[4]; //4  WORD
  ulong  L[2]; //2 DWORD
  signed long long int Q; // QWORD
}bauqword;

typedef
union biauint
{ // Byte-addressable UINT
  uint i; // int: Var.i
  uchar b[2]; // u char: Var.b[0]:Var.b[1]
  struct
  {
    unsigned b0 : 1;
    unsigned b1 : 1;
    unsigned b2 : 1;
    unsigned b3 : 1;
    unsigned b4 : 1;
    unsigned b5 : 1;
    unsigned b6 : 1;
    unsigned b7 : 1;
    unsigned b8 : 1;
    unsigned b9 : 1;
    unsigned b10 : 1;
    unsigned b11 : 1;
    unsigned b12 : 1;
    unsigned b13 : 1;
    unsigned b14 : 1;
    unsigned b15 : 1;
  }bits;
} biauint;

typedef
union biauchar
{ // Byte-addressable UINT
  uchar b;
  struct
  {
    unsigned b0 : 1;
    unsigned b1 : 1;
    unsigned b2 : 1;
    unsigned b3 : 1;
    unsigned b4 : 1;
    unsigned b5 : 1;
    unsigned b6 : 1;
    unsigned b7 : 1;
  }bits;
} biauchar;

typedef union
{
  vu32	   i;
  struct
  {
//r0000
    unsigned f0 : 1;//
    unsigned f1 : 1;//
    unsigned f2 : 1;//
    unsigned f3 : 1;//
    unsigned f4 : 1;//
    unsigned f5 : 1;//
    unsigned f6 : 1;//
    unsigned f7 : 1;
    unsigned f8 : 1;//
    unsigned f9 : 1;//
    unsigned f10 : 1;//
    unsigned f11 : 1;//
    unsigned f12 : 1;//          
    unsigned f13 : 1;//
    unsigned f14 : 1;//
    unsigned f15 : 1;//
//r0001
    unsigned f16 : 1;//
    unsigned f17 : 1;//
    unsigned f18 : 1;//
    unsigned f19 : 1;//
    unsigned f20 : 1;//
    unsigned f21 : 1;//
    unsigned f22 : 1;//
    unsigned f23 : 1;//
    unsigned f24 : 1;//
    unsigned f25 : 1;//
    unsigned f26 : 1;//
    unsigned f27 : 1;//
    unsigned f28 : 1;//
    unsigned f29 : 1;//
    unsigned f30 : 1;//
    unsigned f31 : 1;//
  } BA;
} _FLG0;//флаги управления и индикации;

//параметры расположеные в RAM
struct TRAM_DATA
{
  _FLG0 FLAGS;        //флаги управления и индикации;

  vu16 counter1;
  vu16 counter2;
  vu16 counter3;
  vu16 counter4;
  vu16 data[8];
  vu32 data32[8];

};

#define uart_buff_size 0x0ff // buffer size;
#endif