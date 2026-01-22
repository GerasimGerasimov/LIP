#pragma once

#include "Page/Page.h"

#include <string>
#include <map>

class TMessage;
class ControlSlot;

//содержит Page и управляет страницами
class Router
{
private:
    Router();
    Router(const Router&) = delete;
    Router& operator=(const Router&) = delete;
    Router(const Router&&) = delete;
    Router& operator=(const Router&&) = delete;
    Buffer bufferData; 
    std::vector<std::string> PageList; //Список страниц
    std::map<std::string, ControlSlot*> IndicatorSlots; //Слоты для индикаторов LIP_SS8_Bl_2R
    uint16_t countUpdateSlot = 0;
    void setPage();
    void setPageList();
    void setIndicatorSlots();
    bool updateIndicatorSlots();
    void stopIndicatorSlots();
    void startIndicatorSlots();
public:
    struct Task
    {
        bool isDone;
        std::string NextPage;
        void* props;
    };

    static Router& getInstance();
    Page page; //Текущая страница объект
    int currentPage; //Индекс текущей страницы в списке страниц
    void ProcessMessage(TMessage* m);
    void setTask(Task task);
    void update();
    void setEmptyBufferStatus();
    bool isFillBuffer();
    uint16_t getBufferSize();
    ControlSlot* getAppSlot(std::string name);
    ~Router();
};

