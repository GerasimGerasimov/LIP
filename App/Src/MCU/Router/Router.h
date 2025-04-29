#pragma once

#include "Page/Page.h"

#include <string>
#include <map>

class TMessage;
class ControlIndicatorSlot;

//содержит Page и управляет страницами
class Router
{
private:
    Router();
    Router(const Router&) = delete;
    Router& operator=(const Router&) = delete;
    Router(const Router&&) = delete;
    Router& operator=(const Router&&) = delete;
    //Buffer bufferSender;
    Buffer bufferData;
    std::vector<std::string> Pages;
    std::map<std::string, ControlIndicatorSlot*> IndicatorSlots;
    uint16_t countUpdateSlot = 0;
    void setPage();
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
    Page page;
    int currentPage;
    void ProcessMessage(TMessage* m);
    void setTask(Task task);
    void update();
    void setEmptyBufferStatus();
    bool isFillBuffer();
    uint16_t getBufferSize();
    ControlIndicatorSlot* getAppSlot(std::string name);
    ~Router();
};

