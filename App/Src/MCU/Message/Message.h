//служба сообщений
#ifndef MSG_H
#define MSG_H

#include "stm32f0xx.h"

#include <array>

//defines
#define MAXMESSAGE  0xFFFF


enum class Event
{
    NONE,
    KEYBOARD,
    REPAINT,
    TIMER,
    DATAUPDATE
};

//Клавиатура
/*коды клавиатуры
   P |  UP   | ESC
--------------------
LEFT | ENTER | RIGHT
--------------------
  F1 | DOWN  |   F2
*/

enum class KeyCodes : u32
{
    None = 0, //ни одна клавиша не нажата
    Down = 1,// //стрелка вниз
    Up = 2, //2 //стрелка вверх
};

enum class KeyPressFeature : u32
{
    FirstPress,
    AutoRepeat
};

struct TMessage
{//структура сообщения
    Event event = Event::NONE;//тип сообщения
    //параметры сообщения
    u32 p1 = 0;//первый параметр
    u32 p2 = 0;//второй параметр
};

#define msg_buff_size 32 //размер буфера сообщений

class LipMessage
{
public:
    static LipMessage& getInstance();
    void send_message(Event event, u32 p1, u32 p2);//добавить сообщение в конец очереди
    bool get_message(TMessage* m);//извлечь первое в очереди сообщение
    void clear_msg_queue();//очистка очереди сообщений
private:
    LipMessage();
    LipMessage(const LipMessage&) = delete;
    LipMessage& operator=(const LipMessage&) = delete;
    LipMessage(const LipMessage&&) = delete;
    LipMessage& operator=(const LipMessage&&) = delete;

    TMessage Messages[msg_buff_size];//очередь сообщений
    u32 msg_queue_end = 0;//конец очереди
    u32 msg_queue_beg = 0;//начало очереди
};


namespace KeyBoard
{
    extern std::array<KeyCodes, 2> keyKodes;
}


#endif