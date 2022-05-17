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
       // printf("\n\n\nhands:%d, fingers:%2d, gestures:%d\n",
         //   handList.count(), allFingerList.count(), gesturesInFrame.count());

        int i, j;

        /********************************************
        * 頂点を更新する前に保存 （更新中は保存しない）
        *******************************************/
        if (movingFlag == -1 && rotatingFlag == -1 && scalingFlag == -1) {
            for (int i = 0; i < latitudeNUM + 1; i++) {
                for (int j = 0; j < longitudeNUM; j++) {
                    prePoint[i][j] = point[i][j];
                }
            }
        }
        



        //手と指の処理
        for (i = 0; i < handList.count(); i++) {
            Hand hand = handList[i];
            Vector handCenter = hand.palmPosition();
            FingerList fingerList = hand.fingers();// handの指の情報を取得する

            Vector posi0, posi1;
            Vector Center = { 0,0,0 };

            double pick = 100;


            //個別の手の情報を出力する
            //printf("  hand[%d] (%6.1f,%6.1f,%6.1f), fingers:%d\n",
               // i, handCenter.x, handCenter.y, handCenter.z, fingerList.count());
            for (j = 0; j < fingerList.count(); j++) {
                Finger finger = fingerList[j];
                Vector currentPosition = finger.tipPosition();


                if (j == 0) {
                    posi0 = currentPosition/8;
                    posi0.y -= 30;
                    posi0.z += 8;
                }
                else if (j == 1) {
                    posi1 = currentPosition/8;
                    posi1.y -= 30;
                    posi1.z += 8;
                }

                //個別の指の情報を出力する
               // printf("    finger[%d] (%6.1f,%6.1f,%6.1f)\n",
                 //   j, currentPosition.x , currentPosition.y, currentPosition.z);
            }
            
            Center = (posi0 + posi1) / 2;
            pick = posi0.distanceTo(posi1);
            

            /*　頂点座標の位置の移動 */
            if (scalingFlag == -1) {
                rotateStart = (rotateEnd.y == 999) ? Center : rotateStart;
                rotateEnd = Center;
                for (int j = 0; j < latitudeNUM + 1; j++) {
                    for (int k = 0; k < longitudeNUM + 1; k++) {
                        if (hand.isLeft()) {
                            /*********************************************
                            * 左手で頂点の移動
                            *********************************************/
                            pointer = Center / 2;
                            double dist = point[j][k].distanceTo(Center / 2);
                            //printf("pick:%f, dist:%f\n", pick, dist);
                            if (movingFlag == -1 || movingFlag == j * 100 + k) {
                                if (dist < 1 && pick < 5) {
                                    printf("picked!!\n");
                                    movingFlag = j * 100 + k;
                                     
                                    /* 摘まんだ点の動き */
                                    point[j][k] = Center / 2;

                                    /* 周辺の点の動き */
                                    double TFrate = point[j][k].distanceTo(prePoint[j][k]);       //摘まんだ点の変化に合わせて、周囲の点の変化率を変える
                                    
                                    if (j == 0 || j == latitudeNUM) {   /* 摘まんだ点がpoleだった時 */
                                        int lati1 = (j == 0) ? (1) : (latitudeNUM - 1);
                                        int lati2 = (j == 0) ? (2) : (latitudeNUM - 2);
                                        for (int l = 0; l < longitudeNUM; l++) {
                                            point[lati1][l] = prePoint[lati1][l] + (point[j][k] - prePoint[lati1][l]) * 0.5 * (abs(TFrate) / 6);
                                            point[lati2][l] = prePoint[lati2][l] + (point[j][k] - prePoint[lati2][l]) * 0.2 * (abs(TFrate) / 6);
                                        }
                                    }
                                    else {
                                        /* 頂点の回りの点の動き */
                                        for (int l = -2; l <= 2; l++) {
                                            if (0 < j + l && j + l < latitudeNUM) {
                                                for (int m = -2; m <= 2; m++) {
                                                    if (0 < k + m && k + m < longitudeNUM) {
                                                        if (l != 0 || m != 0) {
                                                            point[j + l][k + m] = prePoint[j + l][k + m] + (point[j][k] - prePoint[j + l][k + m]) * ((abs(l) == 2 || abs(m)==2)?0.2:0.5) * (abs(TFrate) / 6);
                                                        }
                                                        
                                                    }
                                                }
                                            }
                                            
                                        }
                                    }
                                    break;
                                }
                                else {
                                    movingFlag = -1;
                                }
                            }
                        }
                        else {
                            /********************************************
                            * 右手で図形の回転
                            *********************************************/

                            if (pick < 5) {
                                printf("%d\n\n", j);

                                /* マウスボタンが押されてからの移動量を計算する */
                                Vector Move = rotateEnd - rotateStart;


                                ////* x軸周りに回転 *////
                                double RotateX = atan(point[j][k].y / abs(point[j][k].z));    /* 頂点の位置（x軸周りの回転角） */
                                int ZMark = (point[j][k].z < 0) ? -1 : 1;     /* zの正負 */
                                double XDistance = (double)pow(point[j][k].y * point[j][k].y + point[j][k].z * point[j][k].z, 0.5);     /* x軸までの距離 */
                                /* Moveのx軸方向のモーメントから回転方向と回転量を計算する */
                                Vector XAxisPerpendicular = { 0,rotateStart.y,rotateStart.z };
                                Vector XMove = { 0,Move.y,Move.z };
                                Vector XMoment = -XAxisPerpendicular.cross(XMove);
                                /* 回転させる */
                                RotateX += (ZMark < 0) ? -0.02 * XMoment.x : 0.02 * XMoment.x;
                                point[j][k].y = XDistance * sin(RotateX);
                                point[j][k].z = XDistance * cos(RotateX) * ZMark;
                                if (j == 0) {
                                    pole[0].y = XDistance * sin(RotateX);
                                    pole[0].z = XDistance * cos(RotateX) * ZMark;
                                    pole[1].y = XDistance * sin(RotateX);
                                    pole[1].z = XDistance * cos(RotateX) * ZMark;
                                }


                                ////* y軸周りに回転 *////
                                double RotateY = atan(point[j][k].z / abs(point[j][k].x));    /* 頂点の位置（y軸周りの回転角） */
                                int XMark = (point[j][k].x < 0) ? -1 : 1;     /* zの正負 */
                                double YDistance = (double)pow(point[j][k].z * point[j][k].z + point[j][k].x * point[j][k].x, 0.5);     /* y軸までの距離 */
                                /* Moveのy軸方向のモーメントから回転方向と回転量を計算する */
                                Vector YAxisPerpendicular = { rotateStart.x,0,rotateStart.z };
                                Vector YMove = { Move.x,0,Move.z };
                                Vector YMoment = -YAxisPerpendicular.cross(YMove);
                                /* 回転させる */
                                RotateY += (XMark < 0) ? -0.02 * YMoment.y : 0.02 * YMoment.y;
                                point[j][k].z = YDistance * sin(RotateY);
                                point[j][k].x = YDistance * cos(RotateY) * XMark;
                                if (j == 0) {
                                    pole[0].z = YDistance * sin(RotateY);
                                    pole[0].x = YDistance * cos(RotateY) * XMark;
                                    pole[1].z = YDistance * sin(RotateY);
                                    pole[1].x = YDistance * cos(RotateY) * XMark;
                                }


                                ////* z軸周りに回転 *////
                                double RotateZ = atan(point[j][k].x / abs(point[j][k].y));    /* 頂点の位置（z軸周りの回転角） */
                                int YMark = (point[j][k].y < 0) ? -1 : 1;     /* yの正負 */
                                double ZDistance = (double)pow(point[j][k].x * point[j][k].x + point[j][k].y * point[j][k].y, 0.5);     /* z軸までの距離 */
                                /* Moveのz軸方向のモーメントから回転方向と回転量を計算する */
                                Vector ZAxisPerpendicular = { rotateStart.x,rotateStart.y,0 };
                                Vector ZMove = { Move.x,Move.y,0 };
                                Vector ZMoment = ZAxisPerpendicular.cross(ZMove);
                                /* 回転させる */
                                /*RotateZ += (YMark < 0) ? 0.02 * ZMoment.z : -0.02 * ZMoment.z;
                                point[j][k].x = ZDistance * sin(RotateZ);
                                point[j][k].y = ZDistance * cos(RotateZ) * YMark;
                                if (j == 0) {
                                    pole[0].x = ZDistance * sin(RotateZ);
                                    pole[0].y = ZDistance * cos(RotateZ) * YMark;
                                    pole[1].x = ZDistance * sin(RotateZ);
                                    pole[1].y = ZDistance * cos(RotateZ) * YMark;
                                }*/




                                printf("PRE Rotate X:%.1f, Y:%.1f, Z:%.1f x,y,z= %.1f, %.1f, %.1f,  start:%f  end:%f\n",
                                    RotateX, RotateY, RotateZ, point[j][k].x, point[j][k].y, point[j][k].z, rotateStart.y, rotateEnd.y);

                            }
                        }
                    }
                }
            }
            
            /* 次回の回転用に更新 */
            if (hand.isRight()) {
                rotateStart = rotateEnd;
            }

        }
        /******************************
        *手の平を合わせたら、大きさの調整開始
        *********************************/
        if (handList.count() == 2) {
            Hand handR = (handList[0].isRight()) ? handList[0] : handList[1];
            Hand handL = (handList[0].isLeft() ) ? handList[0] : handList[1];
            FingerList RfingerList = handR.fingers();
            FingerList LfingerList = handL.fingers();
            Finger Rfinger = RfingerList[1];
            Finger Lfinger = LfingerList[1];
            Finger Rfinger2 = RfingerList[0];
            Finger Lfinger2 = LfingerList[0];
            Vector Rposi = Rfinger.tipPosition()/8;
            Vector Lposi = Lfinger.tipPosition()/8;
            Vector Rposi2 = Rfinger2.tipPosition()/8;
            Vector Lposi2 = Lfinger2.tipPosition()/8;
            double dist = Rposi.distanceTo(Lposi);
            
            /* 「手のひらを合わせた」かつ「頂点を動かしていない」かつ「回転してない」時に大きさを調整 */
            if (dist <  5 && movingFlag == -1 && rotatingFlag == -1 || scalingFlag == 1) {
                movingFlag = -2;
                rotatingFlag - 2;
                scalingFlag = 1;
                
                
                for (int i = 0; i < latitudeNUM + 1; i++) {
                    for (int j = 0; j < longitudeNUM; j++) {
                        point[i][j] = prePoint[i][j] * 0.1 *dist;
                    }
                }
                printf("%f\n", 0.1 * dist);
                double finishR = Rposi2.distanceTo(Rposi);
                double finishL = Lposi2.distanceTo(Lposi);
                if (finishR < 5 || finishL < 5) {
                    movingFlag = -1;
                    rotatingFlag = -1;
                    scalingFlag = -1;
                }
            }
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
