#pragma once

#include "Message/Message.h"

class App
{
private:
    App();
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(const App&&) = delete;
    App& operator=(const App&&) = delete;

    TMessage message;
public:
    static App& getInstance();
    void run();
};

