

#include "spi.h"
#include "DEFINES.h"

/* defines ------------------------------------------------------------------*/
//����� ������ ��� ��������
uint16_t SPI1_Buffer_Tx[32] = {0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C,
                                  0x0D0E, 0x0F10, 0x1112, 0x1314, 0x1516, 0x1718,
                                  0x191A, 0x1B1C, 0x1D1E, 0x1F20, 0x2122, 0x2324,
                                  0x2526, 0x2728, 0x292A, 0x2B2C, 0x2D2E, 0x2F30,
                                  0x3132, 0x3334, 0x3536, 0x3738, 0x393A, 0x3B3C,
                                  0x3D3E, 0x3F40};


      /*  //�������� ������ �� spi
        for (i=0;i<33;i++)     //�������� ������ 33 - ������ ������
        {
          spi_send(SPI1_Buffer_Tx[i]);
//�������� �� �������� ������         
		  data[i] = spi_receve();
          if (data[i] == SPI1_Buffer_Tx[i]) 
          {
            GPIO_ToggleBits(GPIOD, LED_GRN); //������������� ��������� ����
          }
        }
        // ��������� �������� 
        for (i=0;i<33;i++)
        {
          if (data[i] != SPI1_Buffer_Tx[i]) //���� ��� ���� - ���������� ��� �������
          {
			GPIO_ToggleBits(GPIOD, LED_GRN); //������������� ��������� ����
          }
        }*/


//������� �������� //��������� 2 �����
void spi_send(uint16_t data) 
{
    SPI_I2S_SendData(SPI1,data);
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);  // ��� ���� ������ �����
}
//������� ������ //������� 2 �����
uint16_t spi_receve(void) 
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);  // ��� ���� ������ �������
    uint16_t received = SPI_I2S_ReceiveData(SPI2);
    return received;
}


