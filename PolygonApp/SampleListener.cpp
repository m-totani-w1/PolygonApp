#include "Leap.h"
#include "Header.h"
#include "SampleListener.h"

using namespace Leap; 
/***********LeapMotionの処理***************/

void SampleListener::onInit(const Controller& controller) {
    std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
    /* 使用するジェスチャーをセットする */
    controller.enableGesture(Gesture::TYPE_CIRCLE);
    controller.enableGesture(Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
    controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
    std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
    std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
    Frame frame = controller.frame();// 最新のFrameを取得する
    HandList handList = frame.hands();// 全ての手の情報を取得する
    FingerList allFingerList = frame.fingers();// 全ての指の情報を取得する
    GestureList gesturesInFrame = frame.gestures();// 全てのジェスチャーの情報を取得する

    if (handList.count() != 0) {

        //それぞれの個数を表示する
        printf("\n\n\nhands:%d, fingers:%2d, gestures:%d\n",
            handList.count(), allFingerList.count(), gesturesInFrame.count());

        int i, j;



        //手と指の処理
        for (i = 0; i < handList.count(); i++) {
            Hand hand = handList[i];
            Vector handCenter = hand.palmPosition();
            FingerList fingerList = hand.fingers();// handの指の情報を取得する

            Vector posi0, posi1;
            Vector tmpCenter = { 0,0,0 };

            point[5][0] = handCenter.x/8;
            point[5][1] = handCenter.z/8;


            //個別の手の情報を出力する
            printf("  hand[%d] (%6.1f,%6.1f,%6.1f), fingers:%d\n",
                i, handCenter.x, handCenter.y, handCenter.z, fingerList.count());
            for (j = 0; j < fingerList.count(); j++) {
                Finger finger = fingerList[j];
                Vector currentPosition = finger.tipPosition();

                point[j][0] = currentPosition.x/8;
                point[j][1] = currentPosition.z/8;

                if (j == 0) {
                    posi0 = currentPosition;
                    fing[0][0] = posi0.x / 5;
                    fing[0][1] = posi0.y / 5 - 30;
                }
                else if (j == 1) {
                    posi1 = currentPosition;
                    fing[1][0] = posi1.x / 5;
                    fing[1][1] = posi1.y / 5 - 30;
                }

                //個別の指の情報を出力する
                printf("    finger[%d] (%6.1f,%6.1f,%6.1f)\n",
                    j, currentPosition.x , currentPosition.y, currentPosition.z);

            }
            
            //tmpCenter = (posi0 + posi1) / 2;
            //for (int j = 0; j < 5; j++) {
            //    double pick = pow(posi0.x - posi1.x, 2.0) + pow(posi0.y - posi1.y, 2.0);
            //    double dist = pow(point[j][0] - tmpCenter.x / 5, 2.0) + pow(point[j][1] - tmpCenter.y / 5 - 30, 2.0);
            //    printf("pick: %f  dist: %f\n", pick, dist);
            //    if (dist < 1000 && pick < 300) {
            //        point[j][0] = tmpCenter.x / 5;
            //        point[j][1] = tmpCenter.y / 5 - 30;
            //    }
            //}



        }

        //ジェスチャーの処理
        GestureList gestures = frame.gestures();
        for (i = 0; i < gestures.count(); i++) {
            Gesture gesture = gestures[i];

            //ジェスチャーの種類の出力
            printf("  gesture[%d] ", i);
            switch (gesture.type()) {
            case Gesture::TYPE_CIRCLE:
                printf("CIRCLE\n");
                break;
            case Gesture::TYPE_SWIPE:

                printf("SWIPE\n");
                break;
            case Gesture::TYPE_KEY_TAP:
                printf("KEY_TAP\n");
                break;
            case Gesture::TYPE_SCREEN_TAP:
                printf("SCREEN_TAP\n");
                break;
            default:
                printf("unknown\n");
                break;
            }
        }

    }

}

void SampleListener::onFocusGained(const Controller& controller) {
}

void SampleListener::onFocusLost(const Controller& controller) {
}


void CleanupExit() {
    controller.removeListener(listener);
    exit(1);
}
