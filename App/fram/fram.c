/**
  ******************************************************************************
  * @file    fram.�
  * @author  IMD, Erlina
  * @version V1.0.0
  * @date    18-06-2013
  * @brief   ���� �������� ������� ��� ������ � � ������ ���������� �� Fram 
  * 
  ******************************************************************************
  * @��� � ���� ��������:
  * - ������� ���� ������ ����������������, ������� �������� ������������ 
  * ��������� FRAM. ����� � init.c ��������������� ����� ����������������. 
  * - � ����� fram.h ������������� ������� ��� �������� ���������� ��� ������ 
  * ���������� ���������� (���� ����������).
  * - � ������ ������������� ���������� ���������� ������� check_fram(), �������
  * ��������� ������ ����� ���������� � ����������� ������, �� ��� ���������.
  * - ���� ������� ������� ������ �� ����������� flash ������ ����������, 
  * ������� ������� fram_wr_massive ��� ������ � �������� � ��������� ������� 
  * ����� ���������� ���������� ������ �� flash ������ (����� �� ����� 
  * ���������� ������ ���� ���������� ������)
  * - ��� �� ������������� ������� ����� �������� �� ������������� (��������, 
  * ���� flash-������ ���������� - �������� � �� ������� �� ����� ����������, 
  * �������������� ������ � � ������� fram_rd_massive)
  ******************************************************************************
  * @�������:
  *
  * 
  * 
  * 
  * 
  * 
  *
  * 
  ******************************************************************************
  */
#include "fram.h"
#include "ramdata.h"//��������� ��� ������ � �������������� ���������� ���
#include "crc16.h"//������ � ��������� ��� ������� ����������� ����� �������



//����� ��� ����
u8 Fram_Buffer[FramTmpBufferSize];

/*������� ������ ������ ������ �� ���� FM25040*/
/**
  * @brief  �������� ���� ������ �� fram
  * @param  Data_byt - ������ ��� ��������
  * @retval none.
  */
void fram_wr_byte(u8 Data_byt)
{
  //7���
  if((Data_byt & 0x80) == 0) {Fram_SIO_off;}
  else {Fram_SIO_on;}
  fram_pulse(delay_sck);
  //6���
  if((Data_byt & 0x40) == 0) {Fram_SIO_off;}
  else {Fram_SIO_on;}
  fram_pulse(delay_sck);
  //5���
  if((Data_byt & 0x20) == 0) {Fram_SIO_off;}
  else {Fram_SIO_on;}
  fram_pulse(delay_sck);
  //4���
  if((Data_byt & 0x10) == 0) {Fram_SIO_off;}
  else {Fram_SIO_on;}
  fram_pulse(delay_sck);
  //3���
  if((Data_byt & 0x08) == 0) {Fram_SIO_off;}
  else {Fram_SIO_on;}
  fram_pulse(delay_sck);
  //2���
  if((Data_byt & 0x04) == 0) {Fram_SIO_off;}
  else {Fram_SIO_on;}
  fram_pulse(delay_sck);
  //1���
  if((Data_byt & 0x02) == 0) {Fram_SIO_off;}
  else {Fram_SIO_on;}
  fram_pulse(delay_sck);
  //0���
  if((Data_byt & 0x01) == 0) {Fram_SIO_off;}
  else {Fram_SIO_on;}
  fram_pulse(delay_sck);

}
/**
  * @brief  ������������ �������� ������������ fram SCK 1->0
  * @param  Delay - �������� ��� ������������ ������
  * @retval none.
  */
void fram_pulse(u8 Delay)
{
  u8 i;
  for(i=Delay;i;i--){}
  Fram_SCK_on;//on
  for(i=(Delay+Delay);i;i--){}
  Fram_SCK_off;//off
  for(i=Delay;i;i--){} 
}

/**
  * @brief  ��������� ���� ����
  * @param  none 
  * @retval ����������� ���� ������
  */
u8 fram_rd_byte(void)
{
  u8 Data = 0;
  //7���
  if (Fram_SIO_IN)  Data = Data | 0x80; 
  else Data = 0;
  fram_pulse(delay_sck);
  //6���
   if (Fram_SIO_IN)  Data = Data | 0x40;  
  fram_pulse(delay_sck);
  //5���
  if (Fram_SIO_IN)  Data = Data | 0x20; 
  fram_pulse(delay_sck);
  //4���
  if (Fram_SIO_IN)  Data = Data | 0x10; 
  fram_pulse(delay_sck);
  //3���
  if (Fram_SIO_IN)  Data = Data | 0x08; 
  fram_pulse(delay_sck);
  //2���
  if (Fram_SIO_IN)  Data = Data | 0x04; 
  fram_pulse(delay_sck);
  //1���
  if (Fram_SIO_IN)  Data = Data | 0x02; 
  fram_pulse(delay_sck);
  //0���
  if (Fram_SIO_IN) Data = Data | 0x01;  
  fram_pulse(delay_sck);
  return Data;
}

/**
  * @brief  �������� ���� ������ �����
  * @param  *massive - ��������� �� ������, ������ ����� ������ ��� ������
            byte_num - ���������� ������������ ���� (�� 1 �� 256)
            Addr - �����, � �������� �������� ����� ������/���� ���������� (�� 0 �� 255)
            Mem_half - ���������, ���� ����� 0 - �������� ������, 1 - backup 
  * @retval none
  */
void fram_wr_massive(u8 *massive, u16 byte_num, u8 Addr, u8 Mem_half) 
{
  u16 i =0;
  //cs �����
  Fram_CS_Up();
  Fram_CS_Down();
  fram_wr_byte(WRSR);
  fram_wr_byte(0x00);
  Fram_CS_Up();
  //fram_wren: ���������� ������
  Fram_CS_Down();
    fram_wr_byte(WREN);
    Fram_CS_Up();
    Fram_CS_Down();
  if (Mem_half) fram_wr_byte(FWRITE_bkp);
  else fram_wr_byte(FWRITE);
  fram_wr_byte(Addr);
  //� ������ ���������� ����� ������, n ����� ����
  for(i= Addr ;i< byte_num ;i++)
  {
    fram_wr_byte(massive[i]);
  }
  Fram_CS_Up();
}

/**
  * @brief  ������ ������� ������
  * @param  *massive - ��������� �� ������, ������ ����� ������ ��� ������
            byte_num - ���������� ������������ ���� (�� 1 �� 256)
            Addr - �����, � �������� �������� ����� ������/���� ���������� (�� 0 �� 255)
            Mem_half - ���������, ������ ����� 0 - �������� ������, 1 - backup 
  * @retval none
  */                                                                    
void fram_rd_massive(u8 *massive, u16 n, u8 Addr, u8 Mem_half)
{
  u16 i =0;
  Fram_CS_Down();
  for(i=delay;i;i--){}
  if (Mem_half) fram_wr_byte(FREAD_bkp);
  else fram_wr_byte(FREAD);
  fram_wr_byte(Addr);
  for(i=delay;i;i--){}
  for(i=delay;i;i--){}
  Fram_sio_mode(1);//���� ����� �� ���� ���������
  for(i=delay;i;i--){}
  //������ ������ ������
  for(i=Addr;i<n;i++)
  {
    massive[i] = fram_rd_byte();
  }
  for(i=delay;i;i--){}
  Fram_CS_Up();
  Fram_sio_mode(0);//���� ����� �� �s��� ���������
}  

/**
  * @brief  ��������� ������ ������-�������� fram (���� ������ ������ ��� ���)
  * @param  none 
  * @retval ����������� ���� ������
  */
u8 ReadSR(void)   
{   
  u8 cData = 0;   
  Fram_CS_off;   
  fram_wr_byte(RDSR);
  Fram_sio_mode(1);//���� ����� �� ���� ���������
  cData=fram_rd_byte();   
  Fram_CS_on;   
  Fram_sio_mode(0);//���� ����� �� ���� ���������
  return cData;   
}

/**
  * @brief  ���-������ ���������� � 1
  * @param  none 
  * @retval none
  */
void Fram_CS_Up(void)
{
  u8 i =0;
  for(i=delay;i;i--){}
  Fram_CS_on;
  Fram_SCK_on;//on
  Fram_SIO_on;
  for(i=delay;i;i--){}
}

/**
  * @brief  �������� ���-������
  * @param  none 
  * @retval none
  */
void Fram_CS_Down(void)
{
  u8 i =0;
  for(i=delay;i;i--){}
  for(i=delay;i;i--){}
  Fram_SIO_on;
  Fram_SCK_off;
  for(i=delay;i;i--){}
  Fram_CS_off;
  for(i=delay;i;i--){}
}

/**
  * @brief  ��������� ���� �� ����/�����, � ����������� �� ������/������
  * @param  mode - �������� ��������� ���� //0 - �� �����, 1 - �� ����
  * @retval none
  */
void Fram_sio_mode(u8 mode) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�� ��������� �������� 50���
  if(mode == 0) 
  {
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //output-drain
  }
  else 
  {
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//����, ������ ������ �� ��������� ����������
  }
  //���� �� ���� ������,            FRAM_sio ������ �� ����
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  �������� ����
  * @param none
  * @retval CRC_16
  */
void check_fram (void)
{
  
  RAM_DATA.FLAGS.BA.fram_bkp_error = 0;
  RAM_DATA.FLAGS.BA.fram_error = 0;
  
  fram_rd_massive(Fram_Buffer, FramTmpBufferSize, 0x00, fram_sector);//������ �������� ������ ����
  if (crc16((u8*)&Fram_Buffer, FramTmpBufferSize))
  {    
    //����������� ����� �� �������, ������ ����� ��� ����� ��������
    fram_rd_massive(Fram_Buffer, FramTmpBufferSize, 0x00, fram_bkp);//������ ��������� ������ ����
    if (crc16((u8*)&Fram_Buffer, FramTmpBufferSize)) 
    {
      RAM_DATA.FLAGS.BA.fram_bkp_error = 1; //��� ������� ����� ��� ����� ��������
      RAM_DATA.FLAGS.BA.fram_error = 1; 
    }
    else  
    {
      fram_wr_massive(Fram_Buffer, FramTmpBufferSize, 0x00, fram_sector);//�������� ������������ �� ������
      if (crc16((u8*)&Fram_Buffer, FramTmpBufferSize))   RAM_DATA.FLAGS.BA.fram_error = 1; //�� ����������, ������ ������
    }    
  }
  
  return;
}
