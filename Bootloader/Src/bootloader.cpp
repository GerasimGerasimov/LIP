#include "bootloader.h"
#include "crc16.h"
//#include <string.h>
//#include "str.h"
//#include "STM32F4xx_intmash_MBbasicCommands.h"
#include "memutil.h"
#include "bastypes.h"
#include <vector>

//������� ����������
#define BOOT_CMD_CODE_OFFSET            0x02
#define BOOT_CMD_GET_PAGES_LIST         0x00
#define BOOT_CMD_SET_ERASED_PAGES       0x01
#define BOOT_CMD_SET_START_APPLICATION  0x02
#define BOOT_CMD_GET_MEMORY_FROM_ADDR   0x03 //�������� ������� ��������� ���-�� ���� ������ ������� � ���������� ������ 
#define BOOT_CMD_PUT_AREA_CODE          0x04

#define BOOT_PAGES_LIST_HEAD_SIZE    3
#define BOOT_GET_MEM_HEAD_SIZE       5

#define CRC_SIZE 2 //размер crc-2 байта

u16 getPagesList(TClient* Slave);
u16 setErasedPages(TClient* Slave);
u16 readMemoryBlockFromAddr(TClient* Slave);
u16 writeCodeToFlash(TClient* Slave);
u16 startApplication(TClient* Slave);

u16 BootLoader(TClient* Slave){
  u8 cmd = Slave->Buffer[BOOT_CMD_CODE_OFFSET];
  switch (cmd) {
    case BOOT_CMD_GET_PAGES_LIST:
        return getPagesList(Slave);
    case BOOT_CMD_SET_ERASED_PAGES:
        return setErasedPages(Slave);
    case BOOT_CMD_SET_START_APPLICATION:
        return startApplication(Slave);
    case BOOT_CMD_GET_MEMORY_FROM_ADDR:
        return readMemoryBlockFromAddr(Slave);
    case BOOT_CMD_PUT_AREA_CODE:
        return writeCodeToFlash(Slave);
    default:
      return 0;
  }
}

//������:
//01.B0.03.AAAAAAAA.CC��.CRC
//AAAAAAAA - ��������� 32-������ ����� ������� ������
//���� - ������������� ���-�� ���� 0..64�
//�����:
//01.B0.03.CCCC.DD.DD.DD.DD.DD....DD.DD.CRC
//CCCC- ���-�� ���� � ������, ����� ��������� � ��������
//      ���� ���-�� ���� � ������ ������������ MCU
//      ������, ����������� ������, ������ �������������� ������� ���� ������������
//� ������ ������:
//01.B0.03.0000.CRC

u16 readMemoryBlockFromAddr(TClient* Slave) {
  /* TODO change address-endian at backend  */
  const baulong StartAddr = {
    .b[0] = Slave->Buffer[6],
    .b[1] = Slave->Buffer[5],
    .b[2] = Slave->Buffer[4],
    .b[3] = Slave->Buffer[3]
  };
  const bauint count = {
    .b[0] = Slave->Buffer[8],
    .b[1] = Slave->Buffer[7],
  };
  /*TODO  if count is more than the Slave->Buffer length,
          the count should be as the Slave->Buffer lenght*/
//  u8_mem_cpy( (unsigned char *)StartAddr.L, &Slave->Buffer[BOOT_GET_MEM_HEAD_SIZE], count.i);
  Slave->Buffer[3] =  count.b[1];
  Slave->Buffer[4] =  count.b[0];
  
  u16 DataLength = count.i;
  DataLength += BOOT_GET_MEM_HEAD_SIZE;//��������� ����� ���������   
  DataLength += CRC_SIZE;//��������� ����� crc 
//  FrameEndCrc16((u8*)Slave->Buffer, DataLength);
  return DataLength;
}

//�������� ������� ������ �������������� JSON
//0x00000000 HE �������������� JSON,
//������� hex �������� ���� ���������� � ������ � ������� ""
//� ������� � ������ ������������ �������� ������ 

const char PagesList[] = 
"[{\"start\": \"0x08000000\", \"size\": 1023}," \
 "{\"start\": \"0x08000400\", \"size\": 1023}," \
 "{\"start\": \"0x08000800\", \"size\": 1023}," \
 "{\"start\": \"0x08000C00\", \"size\": 1023}," \
 "{\"start\": \"0x08001000\", \"size\": 1023}," \
 "{\"start\": \"0x08001400\", \"size\": 1023}," \
 "{\"start\": \"0x08001800\", \"size\": 1023}," \
 "{\"start\": \"0x08001C00\", \"size\": 1023}," \
 "{\"start\": \"0x08002000\", \"size\": 1023}," \
 "{\"start\": \"0x08002400\", \"size\": 1023}," \
 "{\"start\": \"0x08002800\", \"size\": 1023}," \
 "{\"start\": \"0x08002C00\", \"size\": 1023}," \
 "{\"start\": \"0x08003000\", \"size\": 1023}," \
 "{\"start\": \"0x08003400\", \"size\": 1023}," \
 "{\"start\": \"0x08003800\", \"size\": 1023}," \
 "{\"start\": \"0x08003C00\", \"size\": 1023}," \
 "{\"start\": \"0x08004000\", \"size\": 1023}," \
 "{\"start\": \"0x08004400\", \"size\": 1023}," \
 "{\"start\": \"0x08004800\", \"size\": 1023}," \
 "{\"start\": \"0x08004C00\", \"size\": 1023}," \
 "{\"start\": \"0x08005000\", \"size\": 1023}," \
 "{\"start\": \"0x08005400\", \"size\": 1023}," \
 "{\"start\": \"0x08005800\", \"size\": 1023}," \
 "{\"start\": \"0x08005C00\", \"size\": 1023}," \
 "{\"start\": \"0x08006000\", \"size\": 1023}," \
 "{\"start\": \"0x08006400\", \"size\": 1023}," \
 "{\"start\": \"0x08006800\", \"size\": 1023}," \
 "{\"start\": \"0x08006C00\", \"size\": 1023}," \
 "{\"start\": \"0x08007000\", \"size\": 1023}," \
 "{\"start\": \"0x08007400\", \"size\": 1023}," \
 "{\"start\": \"0x08007800\", \"size\": 1023}," \
 "{\"start\": \"0x08007C00\", \"size\": 1023}," \
 "{\"start\": \"0x08008000\", \"size\": 1023}," \
 "{\"start\": \"0x08008400\", \"size\": 1023}," \
 "{\"start\": \"0x08008800\", \"size\": 1023}," \
 "{\"start\": \"0x08008C00\", \"size\": 1023}," \
 "{\"start\": \"0x08009000\", \"size\": 1023}," \
 "{\"start\": \"0x08009400\", \"size\": 1023}," \
 "{\"start\": \"0x08009800\", \"size\": 1023}," \
 "{\"start\": \"0x08009C00\", \"size\": 1023}," \
 "{\"start\": \"0x0800A000\", \"size\": 1023}," \
 "{\"start\": \"0x0800A400\", \"size\": 1023}," \
 "{\"start\": \"0x0800A800\", \"size\": 1023}," \
 "{\"start\": \"0x0800AC00\", \"size\": 1023}," \
 "{\"start\": \"0x0800B000\", \"size\": 1023}," \
 "{\"start\": \"0x0800B400\", \"size\": 1023}," \
 "{\"start\": \"0x0800B800\", \"size\": 1023}," \
 "{\"start\": \"0x0800BC00\", \"size\": 1023}," \
 "{\"start\": \"0x0800C000\", \"size\": 1023}," \
 "{\"start\": \"0x0800C400\", \"size\": 1023}," \
 "{\"start\": \"0x0800C800\", \"size\": 1023}," \
 "{\"start\": \"0x0800CC00\", \"size\": 1023}," \
 "{\"start\": \"0x0800D000\", \"size\": 1023}," \
 "{\"start\": \"0x0800D400\", \"size\": 1023}," \
 "{\"start\": \"0x0800D800\", \"size\": 1023}," \
 "{\"start\": \"0x0800DC00\", \"size\": 1023}," \
 "{\"start\": \"0x0800E000\", \"size\": 1023}," \
 "{\"start\": \"0x0800E400\", \"size\": 1023}," \
 "{\"start\": \"0x0800E800\", \"size\": 1023}," \
 "{\"start\": \"0x0800EC00\", \"size\": 1023}," \
 "{\"start\": \"0x0800F000\", \"size\": 1023}," \
 "{\"start\": \"0x0800F400\", \"size\": 1023}," \
 "{\"start\": \"0x0800F800\", \"size\": 1023}," \
 "{\"start\": \"0x0800FC00\", \"size\": 1023}," \
 "{\"start\": \"0x08010000\", \"size\": 1023}," \
 "{\"start\": \"0x08010400\", \"size\": 1023}," \
 "{\"start\": \"0x08010800\", \"size\": 1023}," \
 "{\"start\": \"0x08010C00\", \"size\": 1023}," \
 "{\"start\": \"0x08011000\", \"size\": 1023}," \
 "{\"start\": \"0x08011400\", \"size\": 1023}," \
 "{\"start\": \"0x08011800\", \"size\": 1023}," \
 "{\"start\": \"0x08011C00\", \"size\": 1023}," \
 "{\"start\": \"0x08012000\", \"size\": 1023}," \
 "{\"start\": \"0x08012400\", \"size\": 1023}," \
 "{\"start\": \"0x08012800\", \"size\": 1023}," \
 "{\"start\": \"0x08012C00\", \"size\": 1023}," \
 "{\"start\": \"0x08013000\", \"size\": 1023}," \
 "{\"start\": \"0x08013400\", \"size\": 1023}," \
 "{\"start\": \"0x08013800\", \"size\": 1023}," \
 "{\"start\": \"0x08013C00\", \"size\": 1023}," \
 "{\"start\": \"0x08014000\", \"size\": 1023}," \
 "{\"start\": \"0x08014400\", \"size\": 1023}," \
 "{\"start\": \"0x08014800\", \"size\": 1023}," \
 "{\"start\": \"0x08014C00\", \"size\": 1023}," \
 "{\"start\": \"0x08015000\", \"size\": 1023}," \
 "{\"start\": \"0x08015400\", \"size\": 1023}," \
 "{\"start\": \"0x08015800\", \"size\": 1023}," \
 "{\"start\": \"0x08015C00\", \"size\": 1023}," \
 "{\"start\": \"0x08016000\", \"size\": 1023}," \
 "{\"start\": \"0x08016400\", \"size\": 1023}," \
 "{\"start\": \"0x08016800\", \"size\": 1023}," \
 "{\"start\": \"0x08016C00\", \"size\": 1023}," \
 "{\"start\": \"0x08017000\", \"size\": 1023}," \
 "{\"start\": \"0x08017400\", \"size\": 1023}," \
 "{\"start\": \"0x08017800\", \"size\": 1023}," \
 "{\"start\": \"0x08017C00\", \"size\": 1023}," \
 "{\"start\": \"0x08018000\", \"size\": 1023}," \
 "{\"start\": \"0x08018400\", \"size\": 1023}," \
 "{\"start\": \"0x08018800\", \"size\": 1023}," \
 "{\"start\": \"0x08018C00\", \"size\": 1023}," \
 "{\"start\": \"0x08019000\", \"size\": 1023}," \
 "{\"start\": \"0x08019400\", \"size\": 1023}," \
 "{\"start\": \"0x08019800\", \"size\": 1023}," \
 "{\"start\": \"0x08019C00\", \"size\": 1023}," \
 "{\"start\": \"0x0801A000\", \"size\": 1023}," \
 "{\"start\": \"0x0801A400\", \"size\": 1023}," \
 "{\"start\": \"0x0801A800\", \"size\": 1023}," \
 "{\"start\": \"0x0801AC00\", \"size\": 1023}," \
 "{\"start\": \"0x0801B000\", \"size\": 1023}," \
 "{\"start\": \"0x0801B400\", \"size\": 1023}," \
 "{\"start\": \"0x0801B800\", \"size\": 1023}," \
 "{\"start\": \"0x0801BC00\", \"size\": 1023}," \
 "{\"start\": \"0x0801C000\", \"size\": 1023}," \
 "{\"start\": \"0x0801C400\", \"size\": 1023}," \
 "{\"start\": \"0x0801C800\", \"size\": 1023}," \
 "{\"start\": \"0x0801CC00\", \"size\": 1023}," \
 "{\"start\": \"0x0801D000\", \"size\": 1023}," \
 "{\"start\": \"0x0801D400\", \"size\": 1023}," \
 "{\"start\": \"0x0801D800\", \"size\": 1023}," \
 "{\"start\": \"0x0801DC00\", \"size\": 1023}," \
 "{\"start\": \"0x0801E000\", \"size\": 1023}," \
 "{\"start\": \"0x0801E400\", \"size\": 1023}," \
 "{\"start\": \"0x0801E800\", \"size\": 1023}," \
 "{\"start\": \"0x0801EC00\", \"size\": 1023}," \
 "{\"start\": \"0x0801F000\", \"size\": 1023}," \
 "{\"start\": \"0x0801F400\", \"size\": 1023}," \
 "{\"start\": \"0x0801F800\", \"size\": 1023}," \
 "{\"start\": \"0x0801FC00\", \"size\": 1023}]";

u16 getPagesList(TClient* Slave){
  u16 DataLength = 0; //������ ������������ �������
  DataLength = strlen(PagesList);
  //Slave->Buffer[BOOT_PAGES_LIST_DATA_SECTION + 0] = (DataLength >> 8) & 0x00FF;
  //Slave->Buffer[BOOT_PAGES_LIST_DATA_SECTION + 1] = (DataLength) & 0x00FF;
  //DataLength  = 5;
  strcpy((char *) &Slave->Buffer[BOOT_PAGES_LIST_HEAD_SIZE], PagesList);
  DataLength += BOOT_PAGES_LIST_HEAD_SIZE;//��������� ����� ���������   
  DataLength += CRC_SIZE;//��������� ����� crc 
  FrameEndCrc16((u8*)Slave->Buffer, DataLength);
  return DataLength;
}

//�������� �������� �������
//������
//01.B0.01.PS.AAAAAAAA.BBBBBBBB.CCCCCCCC.CRC
//PS - ���-�� �������
//AAAAAAAA.BBBBBBBB.CCCCCCCC -  ������ ����� �������
//
//�����:
//01.B0.01.CRC
std::vector<u32> getPagesAddrList( u8 * buff) {
    u8 NumOfPages = buff[0];
    u32 * pPages = (u32 * ) &buff[1];
    std::vector<u32> Pages (NumOfPages);
    for (auto & page: Pages) {
        page = *pPages ++;
    };
    
    return Pages;
}

FLASH_Status erasePages(const std::vector<u32> Pages) {
  FLASH_Status status;
  StartFlashChange();
    for (const u32 & page: Pages) {
       status = EraseFlashPage(page);
       if (status != FLASH_COMPLETE) break;
    }
  EndFlashChange();
  return status;
}

u16 setErasedPages(TClient* Slave){
  const std::vector<u32> Pages = getPagesAddrList((u8 *) &Slave->Buffer[3]);
  FLASH_Status status = erasePages(Pages);
  Slave->Buffer[4] = status;
  u16 DataLength  = 4;
  DataLength += CRC_SIZE;//��������� ����� crc 
  FrameEndCrc16((u8*)Slave->Buffer, DataLength);
  return DataLength;
}

/*
Respond:
01.B0.04.CCCC.AA AA AA AA.DD.DD.DD.DD.DD....DD.DD.CRC
CCCC - number of data bytes
AAAAAAAA - 32-bit start address
DD... - data bytes

Answer:
01.B0.04
*/
void writeCodeSpase(u32 startAddr, u16 count, u8 * buff) {
  StartFlashChange();
  while (count-- != 0) { 
    FLASH_ProgramOptionByteData(startAddr++, *buff++);
  }
  EndFlashChange();
}

/*TODO Need to rid up an error with length of data more than 240 bytes, because RX/TX buffer have 16KB length.*/

u16 writeCodeToFlash(TClient* Slave) { 
  const bauint count = {
    .b[0] = Slave->Buffer[4],
    .b[1] = Slave->Buffer[3],
  };
  const baulong StartAddr = {
    .b[0] = Slave->Buffer[8],
    .b[1] = Slave->Buffer[7],
    .b[2] = Slave->Buffer[6],
    .b[3] = Slave->Buffer[5]
  };    
  u8 * pData = (u8 * ) &Slave->Buffer[9];
  writeCodeSpase(StartAddr.L, count.i, pData);
  u16 DataLength  = 3;
  DataLength += CRC_SIZE;//crc 
  FrameEndCrc16((u8*)Slave->Buffer, DataLength);
  return DataLength;
}

#define APPLICATION_ADDRESS 0x08008000

void jumpToApplication(void) {
   /* Устанавливаем адрес перехода на основную программу */
   /* Переход производится выполнением функции, адрес которой указывается вручную */
   /* +4 байта потому, что в самом начале расположен указатель на вектор прерывания */ 
   uint32_t jumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4); 
   typedef void(*pFunction)(void);//объявляем пользовательский тип
   pFunction Jump_To_Application = (pFunction) jumpAddress;
   
   /*Сбрасываем всю периферию на APB1 */
   RCC->APB1RSTR = 0xFFFFFFFF; RCC->APB1RSTR = 0x0; 
  /*Сбрасываем всю периферию на APB2 */ 
   RCC->APB2RSTR = 0xFFFFFFFF; RCC->APB2RSTR = 0x0; 
   RCC->APB1ENR = 0x0; /* Выключаем всю периферию на APB1 */ 
   RCC->APB2ENR = 0x0; /* Выключаем всю периферию на APB2 */
   RCC->AHBENR = 0x0; /* Выключаем всю периферию на AHB */

   /* Сбрасываем все источники тактования по умолчанию, переходим на HSI*/
   RCC_DeInit();  
   
   /* Выключаем прерывания */
   __disable_irq(); 
   /* Переносим адрес вектора прерываний */
   SCB->VTOR = APPLICATION_ADDRESS;//
   /* Переносим адрес стэка */ 
    __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS); 
    /* Переходим в основную программу */  
    Jump_To_Application(); 
}

__no_init char BootLoaderStart[6] @ "BOOT_CMD";

bool isBootLoaderMustBeStart(void) {
  if ( ((BootLoaderStart[0] == 0xA5)  &&
       (BootLoaderStart[1]  == 0x5A)  &&
       (BootLoaderStart[2]  == 0xA5)  &&
       (BootLoaderStart[3]  == 0x5A)) &&
       (crc16((unsigned char *) &BootLoaderStart, 6) == 0)
      )
   {
     return true;
   } else {
    return false;
   }
}

typedef struct appCheckInfo {
  u32 AppSize;
  u16 AppCrc;
  u16 AppInfoCrc;
} TAppCheckInfo;

typedef TAppCheckInfo* pAppCheckInfo;

#define APP_INFO_LOCATION 0x08008200
#define APP_INFO_SIZE 8
#define APP_LOCATION APP_INFO_LOCATION + APP_INFO_SIZE

bool isApplicationReadyToStart(void) {
  const pAppCheckInfo AppCheckInfo = (pAppCheckInfo) APP_INFO_LOCATION;
  return (bool)(crc16((unsigned char *) AppCheckInfo, APP_INFO_SIZE) == 0)
         ? (bool)(crc16((unsigned char *) APP_LOCATION, AppCheckInfo->AppSize) 
                   == AppCheckInfo->AppCrc)
         : false; //header is not valid
}

//01.B0.02.CRC
u16 startApplication(TClient* Slave) {
  BootLoaderStart[0] = 0x00;
  BootLoaderStart[1] = 0x00;
  BootLoaderStart[2] = 0x00;
  BootLoaderStart[3] = 0x00; 
  BootLoaderStart[4] = 0x00;
  BootLoaderStart[5] = 0x00;   
  NVIC_SystemReset();
  return 0;
}