#pragma once

class App
{
private:
    App();
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(const App&&) = delete;
    App& operator=(const App&&) = delete;
public:
    static App& getInstance();
    void run();
};

