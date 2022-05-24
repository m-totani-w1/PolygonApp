
#include <windows.h>
#include "Leap.h"
#include "Header.h"
#include "SampleListener.h"
#include "initializePolygon.h"

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

    //それぞれの個数を表示する
    // printf("\n\n\nhands:%d, fingers:%2d, gestures:%d\n",
    //   handList.count(), allFingerList.count(), gesturesInFrame.count());


    /********************************************
    * 頂点を更新する前に保存 （更新中は保存しない）
    *******************************************/
    if (deformFlag == -1 && rotateFlag == -1 && scaleFlag == -1) {
        prePointer[0] = pointer[0];
        prePointer[1] = pointer[1];
        for (int i = 0; i < latitudeNUM + 1; i++) {
            for (int j = 0; j < longitudeNUM; j++) {
                prePoint[i][j] = point[i][j];
            }
        }
    }
    
    if (handList.count()) {
        rotateFlag = -1;       
        scaleFlag = -1;    
        deformFlag = -1;
    }
    if (shape == shadowCube) {

        autoKaiten();
    }
    for (int i = 0; i < handList.count(); i++) {    // 片手づつ処理を行う
        Hand hand = handList[i];
        
    /* 変形 */
        if (hand.isLeft()) {
            if (handList.count() == 1) {
                rotateFlag = -1;        //左手のみなら、回転フラグをー１
                scaleFlag = -1;        //左手のみなら、拡大フラグをー１
            }
            
            if (easyMode) {
                easyHenkei(hand);
            }
            else {
                henkei(hand);
            }
        }
        /* 回転 */
        if (hand.isRight()) {
            if (handList.count() == 1)deformFlag = -1;        //右手のみなら、変形フラグをー１
            if (easyMode) {
                easyKaitenKakudai(hand);
                //easyKakudai(hand);
            }
            else {
                kaiten(hand);
            }
            
        }
    }

    if (handList.count() == 2) {    //両手あるなら
        /* 拡大・縮小 */
        kakudai(handList);
        /* 初期化 */
        //shokika(handList);
        /* 切替 */
        //kirikae(handList);
    }

    //ジェスチャーの処理
    GestureList gestures = frame.gestures();
    for (int i = 0; i < gestures.count(); i++) {
        Gesture gesture = gestures[i];

        //ジェスチャーの種類の出力
        /*
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
        */
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


/***********************************
* 変形
***********************************/
void henkei(Hand hand) {
    double pick = getPick(hand,0);

    double dist = 100;
    int I =NULL, J=NULL ;   /* pointerに最も近い頂点 */

    /* pointerに最も近い頂点を探す */
    Vector tmpNearestPoint = { 0,0,0 };
    for (int i = 0; i < pointRowNum ; i++) {
        for (int j = 0; j < pointColNum; j++) {
            dist = point[i][j].distanceTo(pointer[0]);
            if (dist < tmpNearestPoint.distanceTo(pointer[0])) {
                tmpNearestPoint = point[i][j];
                I = i;
                J = j;

            }
        }
    }

    dist = pointer[0].distanceTo(tmpNearestPoint);
   nearestPoint = tmpNearestPoint;
   
    if (rotateFlag == -1 && scaleFlag == -1 && pick < 4 || deformFlag == I*100+J) {//どの点も変形中でない(-1)、または自身が変形中(j * 100 + k)
        if (deformFlag == -1) { printf("Transform Start!!\n"); }
        deformFlag = I * 100 + J;


        /* 摘まんだ点を動かす */
        printf("%d %d\n", I, J);
        point[I][J] = pointer[0];



        /* ポリゴンの形ごとに特殊な処理が必要な場合 */
        switch (shape) {

        case ball:
            /* 摘まんだ点の周辺も動かす */
            double TFrate;
            TFrate = point[I][J].distanceTo(prePoint[I][J]);       //摘まんだ点の変化に合わせて、周囲の点の変化率を変える
            TFrate = abs(TFrate) / (CameraDistance * 0.7);
            if (I == 0 || I == latitudeNUM) {   /* 摘まんだ点がpoleだった時 */
                int lati1 = (I == 0) ? (1) : (latitudeNUM - 1);
                int lati2 = (I == 0) ? (2) : (latitudeNUM - 2);
                int lati3 = (I == 0) ? (3) : (latitudeNUM - 3);
                for (int l = 0; l < longitudeNUM; l++) {
                    point[lati1][l] = prePoint[lati1][l] + (point[I][J] - prePoint[lati1][l]) * 0.6 * TFrate;
                    point[lati2][l] = prePoint[lati2][l] + (point[I][J] - prePoint[lati2][l]) * 0.25 * TFrate;
                    point[lati3][l] = prePoint[lati3][l] + (point[I][J] - prePoint[lati3][l]) * 0.06 * TFrate;
                }
            }
            else {
                /* 頂点の回りの点の動き */
                for (int i = -3; i <= 3; i++) {
                    if (0 < I + i && I + i < latitudeNUM) {
                        for (int j = -3; j <= 3; j++) {
                            if (i != 0 || j != 0) {
                                if (abs(i) == 3 || abs(j) == 3) {   //頂点の周辺の動き（＊頂点の隣の隣の隣）
                                    point[I + i][(J + j + longitudeNUM) % longitudeNUM] = prePoint[I + i][(J + j + longitudeNUM) % longitudeNUM] + (point[I][J] - prePoint[I + i][(J + j + longitudeNUM) % longitudeNUM]) * 0.06 * TFrate;
                                }
                                else if (abs(i) == 2 || abs(j) == 2) {  //頂点の周辺の動き（＊頂点の隣の隣）
                                    point[I + i][(J + j + longitudeNUM) % longitudeNUM] = prePoint[I + i][(J + j + longitudeNUM) % longitudeNUM] + (point[I][J] - prePoint[I + i][(J + j + longitudeNUM) % longitudeNUM]) * 0.26 * TFrate;
                                }
                                else {  //頂点の周辺の動き（＊頂点の隣）
                                    point[I + i][(J + j + longitudeNUM) % longitudeNUM] = prePoint[I + i][(J + j + longitudeNUM) % longitudeNUM] + (point[I][J] - prePoint[I + i][(J + j + longitudeNUM) % longitudeNUM]) * 0.6 * TFrate;
                                }

                            }
                        }
                    }

                }
            }
            break;

        case cube:

            break;

        case hexagon:
            break;
        default:
            break;

        }
        

        
        
    }

    if (pick >= 4 && deformFlag == I * 100 + J) {
        printf("Transform Finished!!!\n");
        deformFlag = -1;
    }
    
}

/***********************************
* 回転
***********************************/
void kaiten(Hand hand) {

    double pick = getPick(hand, 1);
    

    if (deformFlag == -1 && scaleFlag == -1 && pick < 4 || rotateFlag == 1) {
        if (rotateFlag == -1) { printf("Rotation Start!!!\n"); }
        rotateFlag = 1;
        for (int j = 0; j < pointRowNum; j++) {
            for (int k = 0; k < pointColNum; k++) {
                /* 移動量を計算する */
                Vector Move = pointer[1] - prePointer[1];


                ////* x軸周りに回転 *////
                double RotateX = (prePoint[j][k].z == 0) ? PI / 2 * sign(prePoint[j][k].y) : atan((prePoint[j][k].y / abs(prePoint[j][k].z)));    /* 頂点の位置（x軸周りの角度） */
                int ZMark = (prePoint[j][k].z < 0) ? -1 : 1;     /* zの正負 */
                double XDistance = (double)pow(prePoint[j][k].y * prePoint[j][k].y + prePoint[j][k].z * prePoint[j][k].z, 0.5);     /* x軸までの距離 */

                /* Moveのx軸方向のモーメントから回転方向と回転量を計算する */
                Vector XAxisPerpendicular = { 0,prePointer[1].y,prePointer[1].z};
                Vector XMove = { 0,Move.y,Move.z };
                Vector XMoment = -XAxisPerpendicular.cross(XMove);
                RotateX += (ZMark < 0) ? -0.02 * XMoment.x : 0.02 * XMoment.x;


                /* 回転させる */
                point[j][k].y = XDistance * sin(RotateX);
                point[j][k].z = XDistance * cos(RotateX) * ZMark;



                ////* y軸周りに回転 *////
                double RotateY = (prePoint[j][k].x == 0) ? PI / 2 * sign(prePoint[j][k].x) : atan((point[j][k].z / abs(prePoint[j][k].x)));    /* 頂点の位置（y軸周りの角度） */
                int XMark = (prePoint[j][k].x < 0) ? -1 : 1;     /* ⅹの正負 */
                double YDistance = (double)pow(point[j][k].z * point[j][k].z + prePoint[j][k].x * prePoint[j][k].x, 0.5);     /* y軸までの距離 */

                /* Moveのy軸方向のモーメントから回転方向と回転量を計算する */
                Vector YAxisPerpendicular = { prePointer[1].x,0,prePointer[1].z};
                Vector YMove = { Move.x,0,Move.z };
                Vector YMoment = -YAxisPerpendicular.cross(YMove);
                RotateY += (XMark < 0) ? -0.02 * YMoment.y : 0.02 * YMoment.y;

                /* 回転させる */
                point[j][k].z = YDistance * sin(RotateY);
                point[j][k].x = YDistance * cos(RotateY) * XMark;



                ////* z軸周りに回転 *////
                double RotateZ = (point[j][k].y == 0) ? PI / 2 * sign(point[j][k].y) : atan((point[j][k].x / abs(point[j][k].y)));    /* 頂点の位置（z軸周りの角度） */
                int YMark = (point[j][k].y < 0) ? -1 : 1;     /* yの正負 */
                double ZDistance = (double)pow(point[j][k].x * point[j][k].x + point[j][k].y * point[j][k].y, 0.5);     /* z軸までの距離 */

                /* Moveのz軸方向のモーメントから回転方向と回転量を計算する */
                Vector ZAxisPerpendicular = { prePointer[1].x,prePointer[1].y,0};
                Vector ZMove = { Move.x,Move.y,0 };
                Vector ZMoment = ZAxisPerpendicular.cross(ZMove);
                RotateZ += (YMark < 0) ? 0.02 * ZMoment.z : -0.02 * ZMoment.z;

                /* 回転させる */

                point[j][k].x = ZDistance * sin(RotateZ);
                point[j][k].y = ZDistance * cos(RotateZ) * YMark;



                if (j == 0 && k == 0) {

                    printf("Rotate X:%.1f, Y:%.1f, Z:%.1f\n", XMoment.x, XMoment.y, XMoment.z);
                    printf("Rotate X:%.1f, Y:%.1f, Z:%.1f\n", YMoment.x, YMoment.y, YMoment.z);
                    printf("Rotate X:%.1f, Y:%.1f, Z:%.1f\n\n", ZMoment.x, ZMoment.y, ZMoment.z);
                }


            }
        }
    }
    if (pick >= 4 && rotateFlag == 1) {
        rotateFlag = -1;
        printf("Rotation Finished!!\n");
    }
    

}

/***********************************
* 拡大・縮小
***********************************/
void kakudai(HandList handList) {
    Hand handR = (handList[0].isRight()) ? handList[0] : handList[1];
    Hand handL = (handList[0].isLeft()) ? handList[0] : handList[1];
    FingerList RfingerList = handR.fingers();
    FingerList LfingerList = handL.fingers();

    Vector Rposi0 = RfingerList[0].tipPosition() / 8;       //右親指
    Vector Lposi0 = LfingerList[0].tipPosition() / 8;       //左親指
    Vector Rposi1 = RfingerList[1].tipPosition() / 8;       //右人差し指
    Vector Lposi1 = LfingerList[1].tipPosition() / 8;       //左人差し指
    Vector Rposi2 = RfingerList[2].tipPosition() / 8;       //右中指
    Vector Lposi2 = LfingerList[2].tipPosition() / 8;       //左中指
    Vector Rposi3 = RfingerList[3].tipPosition() / 8;       //右薬指
    Vector Lposi3 = LfingerList[3].tipPosition() / 8;       //左薬指
    Vector Rposi4 = RfingerList[4].tipPosition() / 8;       //右小指
    Vector Lposi4 = LfingerList[4].tipPosition() / 8;       //左小指

    double scale = Rposi1.distanceTo(Lposi1);    //右人差し指と左人差し指の距離
        /* 「手のひらを合わせた」かつ「頂点を動かしていない」かつ「回転してない」時に大きさを調整 */
    if (scale < 4 && deformFlag == -1 && rotateFlag == -1 || scaleFlag == 1) {
        if (scaleFlag == -1) printf("scaling start!!!\n");
        scaleFlag = 1;




        for (int i = 0; i < pointRowNum; i++) {
            for (int j = 0; j < pointColNum; j++) {
                point[i][j] = prePoint[i][j] * 0.1 * scale;
            }
        }
        printf("magnification：%f\n", 0.1 * scale);

    }

    double finishR = Rposi1.distanceTo(Rposi0);     //右人差し指と右親指の距離
    double finishL = Lposi1.distanceTo(Lposi0);     //左人差し指と左親指の距離
    if (finishR < 4 || finishL < 4) {      //摘まむしぐさでスケーリング終了
        deformFlag = -1;
        rotateFlag = -1;
        scaleFlag = -1;
        printf("scaling finished!\n");
    }
}
/***********************************
* 初期化
***********************************/
void shokika(HandList handList) {
    Hand handR = (handList[0].isRight()) ? handList[0] : handList[1];
    Hand handL = (handList[0].isLeft()) ? handList[0] : handList[1];
    FingerList RfingerList = handR.fingers();
    FingerList LfingerList = handL.fingers();

    Vector Rposi0 = RfingerList[0].tipPosition() / 8;       //右親指
    Vector Lposi0 = LfingerList[0].tipPosition() / 8;       //左親指
    Vector Rposi1 = RfingerList[1].tipPosition() / 8;       //右人差し指
    Vector Lposi1 = LfingerList[1].tipPosition() / 8;       //左人差し指
    Vector Rposi2 = RfingerList[2].tipPosition() / 8;       //右中指
    Vector Lposi2 = LfingerList[2].tipPosition() / 8;       //左中指
    Vector Rposi3 = RfingerList[3].tipPosition() / 8;       //右薬指
    Vector Lposi3 = LfingerList[3].tipPosition() / 8;       //左薬指
    Vector Rposi4 = RfingerList[4].tipPosition() / 8;       //右小指
    Vector Lposi4 = LfingerList[4].tipPosition() / 8;       //左小指


    /* 手が重なると、正常に動かないので二次元の距離を考える */
    Rposi3.z = Lposi1.z;
    Lposi3.z = Rposi1.z;


    double init[2] = { Rposi1.distanceTo(Lposi3),Lposi1.distanceTo(Rposi3) };//人差し指と薬指の距離

    if (deformFlag == -1 && rotateFlag == -1 && scaleFlag == -1) { //どの操作もしていないなら

        if (init[0] < 4 || init[1] < 4) {   //薬指と人差し指が近づいたら
            printf("initialize!!!");
            deformFlag = -2;
            rotateFlag = -2;
            scaleFlag = -2;
            switch (shape)
            {
            case ball:
                InitBall();
                break;
            case cube:
                InitCube();
                break;
            case hexagon:
                InitHexagon();
                break;
            default:
                break;
            }
            Sleep(800);
            deformFlag = -1;
            rotateFlag = -1;
            scaleFlag = -1;
        }
    }

}
void shokika() {

    if (deformFlag == -1 && rotateFlag == -1 && scaleFlag == -1) { //どの操作もしていないなら

        printf("initialize!!!");
        deformFlag = -2;
        rotateFlag = -2;
        scaleFlag = -2;
        switch (shape)
        {
        case ball:
            InitBall();
            break;
        case cube:
            InitCube();
            break;
        case hexagon:
            InitHexagon();
            break;
        case shadowCube:
            InitShadowCube();
            break;
        default:
            break;
        }
        Sleep(800);
        deformFlag = -1;
        rotateFlag = -1;
        scaleFlag = -1;
    }

}
/***********************************
* 切替
***********************************/
void kirikae(HandList handList) {
    Hand handR = (handList[0].isRight()) ? handList[0] : handList[1];
    Hand handL = (handList[0].isLeft()) ? handList[0] : handList[1];
    FingerList RfingerList = handR.fingers();
    FingerList LfingerList = handL.fingers();

    Vector Rposi0 = RfingerList[0].tipPosition() / 8;       //右親指
    Vector Lposi0 = LfingerList[0].tipPosition() / 8;       //左親指
    Vector Rposi1 = RfingerList[1].tipPosition() / 8;       //右人差し指
    Vector Lposi1 = LfingerList[1].tipPosition() / 8;       //左人差し指
    Vector Rposi2 = RfingerList[2].tipPosition() / 8;       //右中指
    Vector Lposi2 = LfingerList[2].tipPosition() / 8;       //左中指
    Vector Rposi3 = RfingerList[3].tipPosition() / 8;       //右薬指
    Vector Lposi3 = LfingerList[3].tipPosition() / 8;       //左薬指
    Vector Rposi4 = RfingerList[4].tipPosition() / 8;       //右小指
    Vector Lposi4 = LfingerList[4].tipPosition() / 8;       //左小指


    /* 手が重なると、正常に動かないので二次元の距離を考える */
    Rposi4.z = Lposi1.z;
    Lposi4.z = Rposi1.z;

    double swichPolygon[2] = { Rposi1.distanceTo(Lposi4),Lposi1.distanceTo(Rposi4) };//人差し指と薬指の距離

    if (deformFlag == -1 && rotateFlag == -1 && scaleFlag == -1) { //どの操作もしていないなら
        if (swichPolygon[0] < 4 || swichPolygon[1] < 4) {   //薬指と人差し指が近づいたら
            printf("switch!!!");
            deformFlag = -2;
            rotateFlag = -2;
            scaleFlag = -2;
            switch (shape)
            {
            case ball:
                InitCube();
                break;
            case cube:
                InitHexagon();
                break;
            case hexagon:
                InitShadowCube();
                break;
            case shadowCube:
                InitBall();
                break;
            default:
                break;
            }
            Sleep(800);
            deformFlag = -1;
            rotateFlag = -1;
            scaleFlag = -1;
        }
    }
}
void kirikae(int i) {

    
    if (deformFlag == -1 && rotateFlag == -1 && scaleFlag == -1) { //どの操作もしていないなら
        printf("switch!!!");
        deformFlag = -2;
        rotateFlag = -2;
        scaleFlag = -2;

        if (i <0) {
            int tmp = (int)shape;
            tmp -= 2;
            shape = (polygon)tmp;
        }
        
        switch (shape)
        {
        case ball:
            InitCube();
            break;
        case cube:
            InitHexagon();
            break;
        case hexagon:
            InitShadowCube();
            break;
        case shadowCube:
            InitBall();
        default:
            break;
        }
        Sleep(800);
        deformFlag = -1;
        rotateFlag = -1;
        scaleFlag = -1;
    }
}

/**************************************
* 簡単モード関数
**************************************/
void easyHenkei(Hand hand) {
    
    double pick = getPick(hand,0);
    double dist = 100;

    /* 変形量計算 */

    double scale = 1 + (pointer[0].z - prePointer[0].z) / 30;

    printf("scale::%f\n", scale);

    if (rotateFlag == -1 && scaleFlag == -1 && pick <= 4 || deformFlag == 1) {//どの点も変形中でない(-1)、または自身が変形中(j * 100 + k)

        if (deformFlag == -1) { printf("Transform Start!!\n"); }
        deformFlag = 1;

        for (int i = 0; i < pointRowNum; i++) {
            for (int j = 0; j < pointColNum; j++) {



                /* ポリゴンの形ごとに特殊な処理が必要な場合 */
                switch (shape) {
                case ball:
                    /* スパイクするように点を動かす */
                    if ((i + j + 1) % 2 == 0 && (i + 1) % 2 == 0) {

                        point[i][j] = prePoint[i][j] * scale;
                    }
                    break;
                case cube:
                    /* つぶれるように点を動かす */


                    point[i][j].x = prePoint[i][j].x * scale;
                    point[i][j].z = prePoint[i][j].z * scale;

                    point[i][j].y = prePoint[i][j].y / (scale);

                    break;
                case hexagon:
                    if (j % 2) {

                        point[i][j].x = prePoint[i][j].x * scale;
                        point[i][j].y = prePoint[i][j].y * scale;
                    }
                    else {
                        point[i][j].x = prePoint[i][j].x / scale;
                        point[i][j].y = prePoint[i][j].y / scale;
                    }
                    break;
                case shadowCube:
                    /* つぶれるように点を動かす */


                    point[i][j].x = prePoint[i][j].x / scale;
                    point[i][j].z = prePoint[i][j].z * scale;

                    point[i][j].y = prePoint[i][j].y / (scale);

                    break;
                default:
                    break;
                }


            }
        }



    }
    if(pick > 4 && deformFlag == 1) {
        printf("Transform Finished!!!\n");
        deformFlag = -1;
        
    }
}
void easyKaitenKakudai(Hand hand) {
    
    double pick = getPick(hand,1);

    if (deformFlag == -1 && pick <= 4 || (rotateFlag == -1 && scaleFlag == 2)) {
        if (rotateFlag == -1 && scaleFlag == -1) { printf("Rotation and Scaling Start!!!\n"); }
        rotateFlag = 1;
        scaleFlag = 2;
        /* 拡大倍率を計算 */
        double scale = 1 + (pointer[1].y - prePointer[1].y) / 30;
        /* 移動量を計算 */
        double MoveX = pointer[1].x - prePointer[1].x;

        for (int i = 0; i < pointRowNum; i++) {
            for (int j = 0; j < pointColNum; j++) {

                point[i][j] = scale * prePoint[i][j];

                
                ////* y軸周りに回転 *////
                double RotateY = (point[i][j].x == 0) ? PI / 2 * sign(point[i][j].x) : atan((point[i][j].z / abs(point[i][j].x)));    /* 頂点の位置（y軸周りの角度） */
                int XMark = (point[i][j].x < 0) ? -1 : 1;     /* ⅹの正負 */
                double YDistance = (double)pow(point[i][j].z * point[i][j].z + point[i][j].x * point[i][j].x, 0.5);     /* y軸までの距離 */

                RotateY -= 0.1 * XMark * MoveX ;

                /* 回転させる */
                point[i][j].z = YDistance * sin(RotateY);
                point[i][j].x = YDistance * cos(RotateY) * XMark;

            }
        }
    }
    if(pick > 4 && rotateFlag == 1 && scaleFlag == 2) {
        rotateFlag = -1;
        scaleFlag = -1;
        
        printf("Rotation and Scaling Finished!!\n");
    }

}
void autoKaiten() {
    if (deformFlag == -1) {
        for (int i = 0; i < pointRowNum; i++) {
            for (int j = 0; j < pointColNum; j++) {


                ////* x軸周りに回転 *////
                double RotateX = (point[i][j].z == 0) ? PI / 2 * sign(point[i][j].y) : atan((point[i][j].y / abs(point[i][j].z)));    /* 頂点の位置（x軸周りの角度） */
                int ZMark = (point[i][j].z < 0) ? -1 : 1;     /* zの正負 */
                double XDistance = (double)pow(point[i][j].y * point[i][j].y + point[i][j].z * point[i][j].z, 0.5);     /* x軸までの距離 */

                RotateX -= 0.01 * ZMark;

                /* 回転させる */
                point[i][j].y = XDistance * sin(RotateX);
                point[i][j].z = XDistance * cos(RotateX) * ZMark;


                ////* z軸周りに回転 *////
                double RotateZ = (point[i][j].y == 0) ? PI / 2 * sign(point[i][j].y) : atan((point[i][j].x / abs(point[i][j].y)));    /* 頂点の位置（z軸周りの角度） */
                int YMark = (point[i][j].y < 0) ? -1 : 1;     /* yの正負 */
                double ZDistance = (double)pow(point[i][j].x * point[i][j].x + point[i][j].y * point[i][j].y, 0.5);     /* z軸までの距離 */

                RotateZ -= 0.01 * YMark;

                /* 回転させる */

                point[i][j].x = ZDistance * sin(RotateZ);
                point[i][j].y = ZDistance * cos(RotateZ) * YMark;



            }
        }
    }
    
}
double getPick(Hand hand,int handNum) {
    /********************************************
    * 手の情報を処理
    ********************************************/
    Vector handCenter = hand.palmPosition();
    FingerList fingerList = hand.fingers();// handの指の情報を取得する

    Vector posi0, posi1;
    Vector Center = { 0,0,0 };

    double pick = 100;

    //個別の手の情報を出力する
    //printf("  hand[%d] (%6.1f,%6.1f,%6.1f), fingers:%d\n",
    // i, handCenter.x, handCenter.y, handCenter.z, fingerList.count());
    for (int j = 0; j < fingerList.count(); j++) {
        Finger finger = fingerList[j];
        Vector currentPosition = finger.tipPosition();

        if (j == 0) {
            posi0 = currentPosition / 8;
            posi0.y -= 30;
            posi0.z += 8;
        }
        else if (j == 1) {
            posi1 = currentPosition / 8;
            posi1.y -= 30;
            posi1.z += 8;
        }

        //個別の指の情報を出力する
        // printf("    finger[%d] (%6.1f,%6.1f,%6.1f)\n",
        //   j, currentPosition.x , currentPosition.y, currentPosition.z);
    }

    Center = (posi0 + posi1) / 2;
    pick = posi0.distanceTo(posi1);
    pointer[handNum] = Center / 2;
    return pick;
}
