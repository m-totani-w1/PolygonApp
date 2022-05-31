
#include "Leap.h"
#include "CommonVariables.h"

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

extern Controller controller;
extern SampleListener listener;
extern Screen screen;
void CleanupExit();

void henkei(Hand hand);
void kaiten(Hand hand);
void kakudai(HandList handList);
void shokika();
void kirikae();

double getPick(Hand hand);

/* 簡単モード */
void easyHenkei(Hand hand);
void easyKaitenKakudai(Hand hand);
void autoKaiten();

/* グローバル変数の更新 */
double udateNearestPoint(int* I, int* J);
void updatePrePoint();