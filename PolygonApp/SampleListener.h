#pragma once
#include "Leap.h"
#include "Header.h"

#ifndef SL_H
#define SL_H

class SampleListener : public Listener {
public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
};

#endif // !SL_H

//必要だろうか。。。？
extern Controller controller;
extern SampleListener listener;
extern Screen screen;
void CleanupExit();
