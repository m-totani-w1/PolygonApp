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
            Vector Center = { 0,0,0 };

            double pick = 100;


            //個別の手の情報を出力する
            printf("  hand[%d] (%6.1f,%6.1f,%6.1f), fingers:%d\n",
                i, handCenter.x, handCenter.y, handCenter.z, fingerList.count());
            for (j = 0; j < fingerList.count(); j++) {
                Finger finger = fingerList[j];
                Vector currentPosition = finger.tipPosition();


                if (j == 0) {
                    posi0 = currentPosition/8;
                    posi0.y -= 30;
                }
                else if (j == 1) {
                    posi1 = currentPosition/8;
                    posi1.y -= 30;
                }

                //個別の指の情報を出力する
                printf("    finger[%d] (%6.1f,%6.1f,%6.1f)\n",
                    j, currentPosition.x , currentPosition.y, currentPosition.z);
            }
            
            Center = (posi0 + posi1) / 2;
            pick = pow(posi0.x - posi1.x, 2.0) + pow(posi0.z - posi1.z, 2.0);

            rotateStart = (rotateEnd.y == 999) ? Center:rotateStart;
            rotateEnd = Center;
            
            for (int j = 0; j < pointNUM; j++) {
                if (hand.isLeft()) {//左手で頂点の移動
                    pointer = Center/2;
                    double dist = point[j].distanceTo(Center/2);
                    printf("pick:%f, dist:%f\n", pick, dist);
                    if (movingFlag == -1 || movingFlag == j) {
                        if (dist < 1 && pick < 20) {
                            printf("picked!!");
                            movingFlag = j;
                            point[j].x = Center.x/2;
                            point[j].y = Center.y/2;
                            point[j].z = Center.z/2;
                            break;
                        }
                        else {
                            movingFlag = -1;
                        }
                    }
                }
                else {//右手で図形の回転

                    if (pick < 20) {
                        printf("%d\n\n", j);

                        /* マウスボタンが押されてからの移動量を計算する */
                        Vector Move = rotateEnd - rotateStart;


                        ////* x軸周りに回転 *////
                        double RotateX = atan(point[j].y / abs(point[j].z));    /* 頂点の位置（x軸周りの回転角） */
                        int ZMark = (point[j].z < 0) ? -1 : 1;     /* zの正負 */
                        double XDistance = (double)pow(point[j].y * point[j].y + point[j].z * point[j].z, 0.5);     /* x軸までの距離 */
                        /* Moveのx軸方向のモーメントから回転方向と回転量を計算する */
                        Vector XAxisPerpendicular = { 0,rotateStart.y,rotateStart.z };
                        Vector XMove = { 0,Move.y,Move.z };
                        Vector XMoment = XAxisPerpendicular.cross(XMove);
                        /* 回転させる */
                        RotateX += (ZMark < 0) ? -0.02 * XMoment.x : 0.02 * XMoment.x;
                        point[j].y = XDistance * sin(RotateX);
                        point[j].z = XDistance * cos(RotateX) * ZMark;


                        ////* y軸周りに回転 *////
                        double RotateY = atan(point[j].z / abs(point[j].x));    /* 頂点の位置（y軸周りの回転角） */
                        int XMark = (point[j].x < 0) ? -1 : 1;     /* zの正負 */
                        double YDistance = (double)pow(point[j].z * point[j].z + point[j].x * point[j].x, 0.5);     /* y軸までの距離 */
                        /* Moveのy軸方向のモーメントから回転方向と回転量を計算する */
                        Vector YAxisPerpendicular = { rotateStart.x,0,rotateStart.z };
                        Vector YMove = { Move.x,0,Move.z };
                        Vector YMoment = YAxisPerpendicular.cross(YMove);
                        /* 回転させる */
                        RotateY += (XMark < 0) ? -0.02 *YMoment.y : 0.02 * YMoment.y;
                        point[j].z = YDistance * sin(RotateY);
                        point[j].x = YDistance * cos(RotateY) * XMark;


                        ////* z軸周りに回転 *////
                        double RotateZ = atan(point[j].x / abs(point[j].y));    /* 頂点の位置（z軸周りの回転角） */
                        int YMark = (point[j].y < 0) ? -1 : 1;     /* yの正負 */
                        double ZDistance = (double)pow(point[j].x * point[j].x + point[j].y * point[j].y, 0.5);     /* z軸までの距離 */
                        /* Moveのz軸方向のモーメントから回転方向と回転量を計算する */
                        Vector ZAxisPerpendicular = { rotateStart.x,rotateStart.y,0 };
                        Vector ZMove = { Move.x,Move.y,0 };
                        Vector ZMoment = ZAxisPerpendicular.cross(ZMove);
                        /* 回転させる */
                        RotateZ += (YMark < 0) ? 0.02 * ZMoment.z  : -0.02 * ZMoment.z;
                        point[j].x = ZDistance * sin(RotateZ);
                        point[j].y = ZDistance * cos(RotateZ) * YMark;




                        printf("PRE Rotate X:%.1f, Y:%.1f, Z:%.1f x,y,z= %.1f, %.1f, %.1f,  start:%f  end:%f\n",
                            RotateX, RotateY, RotateZ, point[j].x, point[j].y, point[j].z, rotateStart.y, rotateEnd.y);


                    }
                }
                
            }
            if (hand.isRight()) {
                rotateStart = (rotateEnd.y < 0) ? Center : rotateEnd;
            }
            
            
            

            
            printf("moving:%d,pointerx:%f,pointery:%f,pointerz:%f", movingFlag,pointer.x, pointer.y, pointer.z);



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
