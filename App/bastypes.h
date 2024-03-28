#ifndef BASTYPES_H
#define BASTYPES_H
#include "stm32f10x.h"// basic types

typedef u8 *pu8;

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long ulong;

typedef
  union bauint { // Byte-addressable UINT
     u16 i; // int: Var.i
     u8 b[2]; // u char: Var.b[0]:Var.b[1]
  } bauint;


typedef
  union bavu16 { // Byte-addressable UINT
     vu16 i; // int: Var.i
     vu8 b[2]; // u char: Var.b[0]:Var.b[1]
  } bavu16;

typedef
  union baulong { // Byte-addressable UINT
     ulong L;    // 1 DWORD
     uint  i[2]; // 2 WORD
     uchar b[4]; // 4 BYTES
  } baulong;

typedef
  union bauqword { // Byte-addressable QWORD 64bit
    uchar  b[8]; //8  BYTES
    uint   i[4]; //4  WORD
    ulong  L[2]; //2 DWORD
    signed long long int Q; // QWORD
  }bauqword;
 
typedef
  union biauint { // Byte-addressable UINT
     uint i; // int: Var.i
     uchar b[2]; // u char: Var.b[0]:Var.b[1]
	 struct {
	 	unsigned b0:1;
	 	unsigned b1:1;
	 	unsigned b2:1;
	 	unsigned b3:1;
	 	unsigned b4:1;
	 	unsigned b5:1;
	 	unsigned b6:1;
	 	unsigned b7:1;
	 	unsigned b8:1;
	 	unsigned b9:1;
	 	unsigned b10:1;
	 	unsigned b11:1;
	 	unsigned b12:1;
	 	unsigned b13:1;
	 	unsigned b14:1;
	 	unsigned b15:1;
	 }bits;
  } biauint;

typedef
  union biauchar { // Byte-addressable UINT
     uchar b;
	 struct {
	 	unsigned b0:1;
	 	unsigned b1:1;
	 	unsigned b2:1;
	 	unsigned b3:1;
	 	unsigned b4:1;
	 	unsigned b5:1;
	 	unsigned b6:1;
	 	unsigned b7:1;
	 }bits;
  } biauchar;

struct TFLASH_DATA {
    // Секция связи
	bavu16 MODBUS1;         //0x0000// device addres and baud rate for uart1 (modbus1);
	bavu16 MODBUS2;         //0x0002// device addres and baud rate for uart2 (modbus2);
    
    // Секция задания
    vu16 Iz;                //0x0004// Iz
    vu16 Uz;                //0x0006// Uz max
    vu16 VA_fall_norm;      //0x0008// скорость спада Альфа нормальная 5%/s=9градусов=500тиков 1s=100синхр,1синхра - 5 тиков
    vu16 VA_rise;           //0x000a// скорость нарастаний  Альфа при остановке
    vu16 VA_fall_spark;     //0x000c скорость спада Альфа после искры 1градус на синхру?=55тиков

    // Секция П-регулятора
    vu16 Ky_U;                //0x000e// Ky
    vu16 Ky_I;                //0x000e// Ky

    // Секция Защиты 
    vu16 Ish_mtz;           //0x0012// Max Ish mtz
    vu16 Iload_mtz;         //0x0014// Max Iload mtz
    vu16 Iload_OPC;         //0x0036// холостой ход
    vu16 Uload_OPC;         //0x0038//    
    vu16 Iload_SHC;         //0x003a// короткое замыкание
    vu16 Uload_SHC;         //0x003c//    
    vu16 Spark_fq;         //0x003c//frequent spark проверка на частые пробои, сколько единиц выставлено

    // Секция Встряхивателя
    vu16 ShPeriod;          //0x0016// shacker periodical  600... sec
    vu16 ShWidth;              //0x0018// shacker width ширина импульса работы встряхивателя 1-5 sec

    // Секция датчика искрового разряда
    vs16 SkipImpSpark;      //0x001e// сколько импульсов синхры пропустить после искры, перед запуском тиристоров
    vu16 Uth;               //0x0020// Treshold Load voltage for ESpark //проверка на искру
    vu16 Ith;               //0x0022// Treshold Load Current for ESpark
    vu16 K_SensSpark;//Koef sensitivity spark чувствительность искры, коэф * число искры = на сколько отбрасывается угол, 1 - 10мкс
    
    // Секция тиристорного регулятора
    vu16 Amax;              //0x0028// 44920 max thyristors angle
    vu16 Amin;              //0x002a// 8 min thyristors angle
    vu16 TirImpWidth;      //0x002c// Tiristor impulse width  ширина управляющего импульса
    vu16 TirPWM_Pulse;     //0x002e// скважность высокочастотного наполнения имульсов тиристоров
    
    // Секция нулей АЦП
    vu16 KADC_Uload;
    vu16 KADC_Iload; //коэффициент для калибровки
    vu16 KADC_Ish;

    vu16 Uload_offs;          //смещение нуля для калибровки
    vu16 Iload_offs;          //0x0032// Iload offset
    vu16 Ish_offs;            //0x0034// Ish   offset
    
    //vu16  koef_Uth;// 208 //0x0036//
   // vu16 koef_Ith;// 150 /* 10% от номинального тока */ //0x0038//
    vu16 Vcc;// 2087//1200 //0x003a//
    vu16 R1;// 26 //0x003c//
    vu16 R2;// 100 //0x003e//
    vu16 R3;// (220+150) //0x0040//
    
   // vu16 Ftlim; //крайнее ограничение

  };




typedef union {
  vu32	   i;
  struct {
    //r0000
     unsigned flash_error : 1;	//
     unsigned backup_error : 1;	//
     unsigned fram_error : 1;	//
     unsigned fram_bkp_error : 1; //
    //флаги DIO
    unsigned DOUT1_FAIL : 1; //РАС аварийное отключение
    unsigned DOUT2_SHAKE: 1; //пока встряхивается встряхиватель  
    unsigned DOUT3_SIGN : 1; //РПС сигнализация 
    unsigned DI1 : 1;	     //discret input 1
    unsigned DI2_BURNING : 1; //значит включен ПРОЖИГ - не ндадо проверять на искру!
    unsigned DI3_STOP : 1;   //Нажата кнопка Стоп 
    unsigned DI4_START : 1;  //Нажата кнопка Пуск
    unsigned DI5_TERM : 1;   //термореле
    //флаги аварий если флаг в 1 - есть авария
    unsigned SYNCF : 1;	    //SYNC FAULT              
    unsigned MTZ_SH : 1;    //mtz по входу, Ish
    unsigned MTZ_L : 1;	    //mtz по нагрузке, Iload
    unsigned SHC : 1;	    //short circuit кз
    unsigned OPC : 1;	    //open  circuit хх
    unsigned OVH    : 1;    //over heating (перегрев) 
    unsigned THFOC  : 1;    //fault thyristors not open(неисправность тиристоров неоткрывается)  оставить но не исп
    unsigned THFAO  : 1;    //fault thyristors not close(неисправность тиристоров незакрывается)  оставить но не исп
    unsigned FQS    : 1;    //frequent spark частые пробои - кол-во пробоев за период
    //r0001
    //прочие флаги 
    unsigned TR_ENABLE : 1; //импульсы на тиpисторы если есть аварии, при которых выкл импульсы
    unsigned TR1  : 1;   //проводимость 1- тиристора
    unsigned TR2  : 1;   ///проводимость 2- тиристора
    unsigned SPARK : 1;  // Пришла искра
    unsigned WAIT : 1;   //лаг - состояние ожидания
    unsigned A_STOP : 1; //флаг изменения/повышения  угла альфа вызова STOP
    unsigned DOUT4_SHAKE2 : 1;    //второй встряхиватель на внешней плате
    unsigned DOUT5 : 1;    //дискр вых допольнительно
    unsigned DOUT6 : 1;	 //дискр вых допольнительно
    unsigned DI6 : 1;	 //дискр вх допольнительно
    unsigned NA8 : 1;    //
  } BA;
} _FLG0;//флаги управления и индикации;

//параметры расположеные в RAM
  struct TRAM_DATA {
   // _FLG0 FLAGS;        //флаги управления и индикации;
    vu16 counter[4];           //
    // Секция измерения за период
     //
    // Секция задания
    vu16 data[8];
    vu32 data32[8];
    vu32 UsedMem; //4 байта
    vu32 TotalMem; //4 байта
    vu32 arena;
    vu32 totalFreeSpace;
    //vu16 reserve;
    vu16 DI;
} ;

struct TCLBR_DATA {
    u16 ThFFDC;//скважность высокочастотного наполнения имульсов тиристоров
    u16 ULoffs;
    u16 ILoffs;
    u16 Iioffs;
 };

#define uart_buff_size 0x0ff // buffer size;
#endif