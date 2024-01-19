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
    // ������ �����
	bavu16 MODBUS1;         //0x0000// device addres and baud rate for uart1 (modbus1);
	bavu16 MODBUS2;         //0x0002// device addres and baud rate for uart2 (modbus2);
    
    // ������ �������
    vu16 Iz;                //0x0004// Iz
    vu16 Uz;                //0x0006// Uz max
    vu16 VA_fall_norm;      //0x0008// �������� ����� ����� ���������� 5%/s=9��������=500����� 1s=100�����,1������ - 5 �����
    vu16 VA_rise;           //0x000a// �������� ����������  ����� ��� ���������
    vu16 VA_fall_spark;     //0x000c �������� ����� ����� ����� ����� 1������ �� ������?=55�����

    // ������ �-����������
    vu16 Ky_U;                //0x000e// Ky
    vu16 Ky_I;                //0x000e// Ky

    // ������ ������ 
    vu16 Ish_mtz;           //0x0012// Max Ish mtz
    vu16 Iload_mtz;         //0x0014// Max Iload mtz
    vu16 Iload_OPC;         //0x0036// �������� ���
    vu16 Uload_OPC;         //0x0038//    
    vu16 Iload_SHC;         //0x003a// �������� ���������
    vu16 Uload_SHC;         //0x003c//    
    vu16 Spark_fq;         //0x003c//frequent spark �������� �� ������ ������, ������� ������ ����������

    // ������ �������������
    vu16 ShPeriod;          //0x0016// shacker periodical  600... sec
    vu16 ShWidth;              //0x0018// shacker width ������ �������� ������ ������������� 1-5 sec

    // ������ ������� ��������� �������
    vs16 SkipImpSpark;      //0x001e// ������� ��������� ������ ���������� ����� �����, ����� �������� ����������
    vu16 Uth;               //0x0020// Treshold Load voltage for ESpark //�������� �� �����
    vu16 Ith;               //0x0022// Treshold Load Current for ESpark
    vu16 K_SensSpark;//Koef sensitivity spark ���������������� �����, ���� * ����� ����� = �� ������� ������������� ����, 1 - 10���
    
    // ������ ������������ ����������
    vu16 Amax;              //0x0028// 44920 max thyristors angle
    vu16 Amin;              //0x002a// 8 min thyristors angle
    vu16 TirImpWidth;      //0x002c// Tiristor impulse width  ������ ������������ ��������
    vu16 TirPWM_Pulse;     //0x002e// ���������� ���������������� ���������� �������� ����������
    
    // ������ ����� ���
    vu16 KADC_Uload;
    vu16 KADC_Iload; //����������� ��� ����������
    vu16 KADC_Ish;

    vu16 Uload_offs;          //�������� ���� ��� ����������
    vu16 Iload_offs;          //0x0032// Iload offset
    vu16 Ish_offs;            //0x0034// Ish   offset
    
    //vu16  koef_Uth;// 208 //0x0036//
   // vu16 koef_Ith;// 150 /* 10% �� ������������ ���� */ //0x0038//
    vu16 Vcc;// 2087//1200 //0x003a//
    vu16 R1;// 26 //0x003c//
    vu16 R2;// 100 //0x003e//
    vu16 R3;// (220+150) //0x0040//
    
   // vu16 Ftlim; //������� �����������

  };




typedef union {
  vu32	   i;
  struct {
    //r0000
     unsigned flash_error : 1;	//
     unsigned backup_error : 1;	//
     unsigned fram_error : 1;	//
     unsigned fram_bkp_error : 1; //
    //����� DIO
    unsigned DOUT1_FAIL : 1; //��� ��������� ����������
    unsigned DOUT2_SHAKE: 1; //���� ������������� �������������  
    unsigned DOUT3_SIGN : 1; //��� ������������ 
    unsigned DI1 : 1;	     //discret input 1
    unsigned DI2_BURNING : 1; //������ ������� ������ - �� ����� ��������� �� �����!
    unsigned DI3_STOP : 1;   //������ ������ ���� 
    unsigned DI4_START : 1;  //������ ������ ����
    unsigned DI5_TERM : 1;   //���������
    //����� ������ ���� ���� � 1 - ���� ������
    unsigned SYNCF : 1;	    //SYNC FAULT              
    unsigned MTZ_SH : 1;    //mtz �� �����, Ish
    unsigned MTZ_L : 1;	    //mtz �� ��������, Iload
    unsigned SHC : 1;	    //short circuit ��
    unsigned OPC : 1;	    //open  circuit ��
    unsigned OVH    : 1;    //over heating (��������) 
    unsigned THFOC  : 1;    //fault thyristors not open(������������� ���������� �������������)  �������� �� �� ���
    unsigned THFAO  : 1;    //fault thyristors not close(������������� ���������� �������������)  �������� �� �� ���
    unsigned FQS    : 1;    //frequent spark ������ ������ - ���-�� ������� �� ������
    //r0001
    //������ ����� 
    unsigned TR_ENABLE : 1; //�������� �� ��p������ ���� ���� ������, ��� ������� ���� ��������
    unsigned TR1  : 1;   //������������ 1- ���������
    unsigned TR2  : 1;   ///������������ 2- ���������
    unsigned SPARK : 1;  // ������ �����
    unsigned WAIT : 1;   //��� - ��������� ��������
    unsigned A_STOP : 1; //���� ���������/���������  ���� ����� ������ STOP
    unsigned DOUT4_SHAKE2 : 1;    //������ ������������� �� ������� �����
    unsigned DOUT5 : 1;    //����� ��� ��������������
    unsigned DOUT6 : 1;	 //����� ��� ��������������
    unsigned DI6 : 1;	 //����� �� ��������������
    unsigned NA8 : 1;    //
  } BA;
} _FLG0;//����� ���������� � ���������;

//��������� ������������ � RAM
  struct TRAM_DATA {
    _FLG0 FLAGS;        //����� ���������� � ���������;
    // ������ �������
    vu16 Iz;
    vu16 Uz;
    vu16 Az;
    // ������ �������������
    vu16 A;             //���� ���������� �����������, �����, ������� ������� ������� � 0
    // ������ ������������ ��������� ��� �������
    vu16 V_Ref;         //���������� ��������
    vu16 _Iload;        //non filtred ��� �������� II
    vu16 _Uload;        //non filtred ���������� ��������
    vu16 _Ish;          //non filtred ��� �������� I
    // ������ �������������� ���������
    vu16 Iload;         //��� ��������
    vu16 Uload;         //���������� ��������
    vu16 counter1;           //
    // ������ ��������� �� ������
    vu16 Iload_avg;        //
    vu16 Uload_avg;        //
    vu16 Ish_avg;          //
    //�������� �� �����
    vu16 Spark_cnt;          //
    
    // ���������� ������ �� ������ - �������� �������� ������, �� ���� ����� ��� ������� - 
    vu16 A_idiot;            //
    vu16 Iload_idiot;        //
    vu16 Uload_idiot;        //
    vu16 Ish_idiot;          //

    
    /*������ ��� ���������*/
vu16 Spark_cnt_view;          //
vu16 Il_buffer;          //
vu16 Ul_buffer;          //
vu16 load_buf; //

//���������� ��� ���������� �� ������� - ����� ����� ���� ��������
    vu16 Iload_Efiltr;        //
    vu16 Uload_Efiltr;        //

    
   // vu16 V_Ref;         //���������� ��������
} ;

struct TCLBR_DATA {
    u16 ThFFDC;//���������� ���������������� ���������� �������� ����������
    u16 ULoffs;
    u16 ILoffs;
    u16 Iioffs;
 };

#define uart_buff_size 0x0ff // buffer size;
#endif