//������ ���������
#ifndef MSG_H
#define MSG_H

#include "stm32f10x.h"

//defines
#define MAXMESSAGE  0xFFFF


enum class EventSrc {
	NONE,
	KEYBOARD,
	REPAINT,
	TIMER,
	DATAUPDATE
};

//����������
/*���� ����������
   P |  UP   | ESC
--------------------
LEFT | ENTER | RIGHT
--------------------
  F1 | DOWN  |   F2
*/

enum class KeyCodes : u32 {
	None    = 0, //�� ���� ������� �� ������
	ESC     = 8, //������ (� ����� �� ���� ������� ���� �����)
	Up      = 16, //2 //������� �����

	Right   = 256, //������� ������
	ENT     = 32,//5 //����� ������
	Left    = 64, //������� �����
	
	Down    = 128,//8 //������� ����
	F1      = 1, //�������������� ������� F1
        F2      = 2, //�������������� ������� F2
        F3      = 4, //�������������� ������� F3
		F3_Left = F3 + Left, //������ ������� F3 � ������� �����
		F3_Right = F3 + Right //������ ������� F3 � ������� ������
};

enum class KeyPressFeature : u32 {
  FirstPress,
  AutoRepeat
};

typedef struct {//���, ����� ���� �������� ;-)
  u16 x;
  u16 y;
  u8  Key;
} TMouseState;

typedef struct {//��������� ���������
  u32 Event;//��� ���������
  //��������� ���������
  u32 p1;//������ ��������
  u32 p2;//������ ��������
} TMessage;

#define msg_buff_size 32 //������ ������ ���������

class Msg {
public:
	static Msg& getInstance();
	void send_message(u32 event, u32 p1, u32 p2);//�������� ��������� � ����� �������
	bool get_message(TMessage* m);//������� ������ � ������� ���������
	void clear_msg_queue();//������� ������� ���������
private:
	Msg();
	Msg(const Msg&) = delete;
	Msg& operator=(const Msg&) = delete;
	Msg(const Msg&&) = delete;
	Msg& operator=(const Msg&&) = delete;

	TMessage Messages[msg_buff_size];//������� ���������
	u32 msg_queue_end = 0;//����� �������
	u32 msg_queue_beg = 0;//������ �������
};

#endif