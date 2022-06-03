
#include <windows.h>
#include "Leap.h"
#include "SampleListener.h"
#include "initializePolygon.h"
#include "Convenient.h"

using namespace Leap;
/***********LeapMotionの処理***************/

void SampleListener::onInit(const Controller& controller) {
    std::cout << "Initialized" << std::endl;
    InitBall();
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
    

    //ポリゴンが陰影立方体ならば、自動回転指せる
    if (SHAPE == SHADOWCUBE) {
        autoKaiten();
    }

    Frame frame = controller.frame();// 最新のFrameを取得する
    HandList handList = frame.hands();// 全ての手の情報を取得する
    FingerList allFingerList = frame.fingers();// 全ての指の情報を取得する
    
    //手が無ければ、どの操作も行われていない
    if (handList.count() == 0) {
        rotateFlag = false;       
        scaleFlag = false;    
        deformFlag = false;
    }

    //手があれば片手ずつ処理をおこなう
    for (int i = 0; i < handList.count(); i++) { 
        Hand hand = handList[i];
        
        /* 変形 */
        if (hand.isLeft()) {
            if (handList.count() == 1) {
                rotateFlag = false;        //左手のみなら、回転フラグをfalse
                scaleFlag = false;        //左手のみなら、拡大フラグをfalse
            }
            
            updatePrePoint();
            if (easyMode) {
                easyHenkei(hand);
            }
            else {
                henkei(hand);
            }
        }

        /* 回転・拡大 */
        if (hand.isRight()) {
            if (handList.count() == 1) {
                deformFlag = false;        //右手のみなら、変形フラグをfalse
            }

            updatePrePoint();

            if (easyMode) {
                easyKaitenKakudai(hand);
            }
            else {
                kaiten(hand);
            }
        }
    }

    //両手ある場合の処理
    if (handList.count() == 2 && !(easyMode)) {    //両手あり、簡単モードで無いなら
        updatePrePoint();
        /* 拡大・縮小 */
        kakudai(handList);
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


/***********************************************************
|  関数：henkei()
|  説明：ポリゴンを変形する関数
|  引数：Hand hand
|  戻値：無し
***********************************************************/
void henkei(Hand hand) {
    double pick = getPick(hand);
    int I, J;
    double dist = updateNearestPoint(&I,&J);
    
    if (!rotateFlag && !scaleFlag && pick < 4 && dist < 2 || deformFlag) {//どの点も変形中でない(-1)、または自身が変形中(j * 100 + k)
        if (!deformFlag) { printf("Transform Start!!\n"); }
        deformFlag = true;


        /* 摘まんだ点を動かす */
        printf("%d %d\n", I, J);
        point[I][J] = pointer[0];



        /* ポリゴンの形ごとに特殊な処理が必要な場合 */
        switch (SHAPE) {

        case BALL:
            /* 摘まんだ点の周辺も動かす */
            double TFrate;
            TFrate = point[I][J].distanceTo(prePoint[I][J]);       //摘まんだ点の変化に合わせて、周囲の点の変化率を変える
            TFrate = abs(TFrate) / 12.5;
            if (I == 0 || I == pointRowNum-1) {   /* 摘まんだ点がpoleだった時 */
                int lati1 = (I == 0) ? (1) : (pointRowNum - 2);
                int lati2 = (I == 0) ? (2) : (pointRowNum - 3);
                int lati3 = (I == 0) ? (3) : (pointRowNum - 4);
                for (int l = 0; l < pointColNum; l++) {
                    point[lati1][l] = prePoint[lati1][l] + (point[I][J] - prePoint[lati1][l]) * 0.6 * TFrate;
                    point[lati2][l] = prePoint[lati2][l] + (point[I][J] - prePoint[lati2][l]) * 0.25 * TFrate;
                    point[lati3][l] = prePoint[lati3][l] + (point[I][J] - prePoint[lati3][l]) * 0.06 * TFrate;
                }
            }
            else {
                /* 頂点の回りの点の動き */
                for (int i = -3; i <= 3; i++) {
                    if (0 < I + i && I + i < pointRowNum-1) {
                        for (int j = -3; j <= 3; j++) {
                            if (i != 0 || j != 0) {
                                if (abs(i) == 3 || abs(j) == 3) {   //頂点の周辺の動き（＊頂点の隣の隣の隣）
                                    point[I + i][(J + j + pointColNum) % pointColNum] = prePoint[I + i][(J + j + pointColNum) % pointColNum] + (point[I][J] - prePoint[I + i][(J + j + pointColNum) % pointColNum]) * 0.06 * TFrate;
                                }
                                else if (abs(i) == 2 || abs(j) == 2) {  //頂点の周辺の動き（＊頂点の隣の隣）
                                    point[I + i][(J + j + pointColNum) % pointColNum] = prePoint[I + i][(J + j + pointColNum) % pointColNum] + (point[I][J] - prePoint[I + i][(J + j + pointColNum) % pointColNum]) * 0.26 * TFrate;
                                }
                                else {  //頂点の周辺の動き（＊頂点の隣）
                                    point[I + i][(J + j + pointColNum) % pointColNum] = prePoint[I + i][(J + j + pointColNum) % pointColNum] + (point[I][J] - prePoint[I + i][(J + j + pointColNum) % pointColNum]) * 0.6 * TFrate;
                                }

                            }
                        }
                    }

                }
            }
            break;

        case CUBE:
            break;

        case HEXAGON:
            break;
        default:
            break;
        }  
    }

    if (pick >= 4 && deformFlag) {
        printf("Transform Finished!!!\n");
        deformFlag = false;
    }
    
}

/***********************************************************
|  関数：kaiten()
|  説明：ポリゴンを回転する関数
|  引数：Hand hand
|  戻値：無し
***********************************************************/
void kaiten(Hand hand) {

    double pick = getPick(hand);

    if (!deformFlag && !scaleFlag && pick < 4 || rotateFlag) {
        if (!rotateFlag) { printf("Rotation Start!!!\n"); }
        rotateFlag = true;
        for (int j = 0; j < pointRowNum; j++) {
            for (int k = 0; k < pointColNum; k++) {

                /* ポインタの移動量を計算する */
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

    if (pick >= 4 && rotateFlag) {
        rotateFlag = false;
        printf("Rotation Finished!!\n");
    }
    

}

/***********************************************************
|  関数：kakudai()
|  説明：ポリゴンを拡大する関数
|  引数：HandList handLisrt
|  戻値：無し
***********************************************************/
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
    if (scale < 4 && !deformFlag && !rotateFlag || scaleFlag) {
        if (!scaleFlag) printf("scaling start!!!\n");
        scaleFlag = true;

        for (int i = 0; i < pointRowNum; i++) {
            for (int j = 0; j < pointColNum; j++) {
                point[i][j] = prePoint[i][j] * 0.1 * scale;
            }
        }
        printf("magnification：%f\n", 0.1 * scale);

    }

    double pickR = getPick(handR);     //右人差し指と右親指の距離
    double pickL = getPick(handL);     //左人差し指と左親指の距離
    if ((pickR < 4 || pickL < 4) && scaleFlag) {      //摘まむしぐさでスケーリング終了
        scaleFlag = false;
        printf("scaling finished!\n");
    }
}
/***********************************************************
|  関数：shokika()
|  説明：ポリゴンを初期化する関数
|  引数：無し
|  戻値：無し
***********************************************************/
void shokika() {

    if (!deformFlag && !rotateFlag && !scaleFlag) { //どの操作もしていないなら

        printf("initialize!!!");

        switch (SHAPE)
        {
        case BALL:
            InitBall();
            break;
        case CUBE:
            InitCube();
            break;
        case HEXAGON:
            InitHexagon();
            break;
        case SHADOWCUBE:
            InitShadowCube();
            break;
        default:
            break;
        }
        Sleep(800);
    }

}
/***********************************************************
|  関数：kirikae()
|  説明：ポリゴンを切り替える関数
|  引数：無し
|  戻値：無し
***********************************************************/
void kirikae() {
    
    if (!deformFlag&& !rotateFlag && !scaleFlag) { //どの操作もしていないなら
        printf("switch!!!");
        
        switch (SHAPE)
        {
        case BALL:
            InitShadowCube();
            break;
        case SHADOWCUBE:
            InitCube();
            break;
        case CUBE:
            InitHexagon();
            break;
        case HEXAGON:
            InitBall();
            break;
        default:
            break;
        }
        Sleep(800);
    }
}

/***********************************************************
|  関数：easyHenkei()
|  説明：ポリゴンを変形する関数（簡単モード）
|  引数：Hand hand
|  戻値：無し
***********************************************************/
void easyHenkei(Hand hand) {
    
    double pick = getPick(hand);

    /* 変形量計算 */
    double scale = 1 + (pointer[0].z - prePointer[0].z) / 30;       //指の移動量から変形具合を計算
    printf("scale::%f\n", scale);
    /* 変形開始 */
    if (!rotateFlag && !scaleFlag && pick <= 4 || deformFlag) {//どの点も変形中でない(-1)、または自身が変形中(j * 100 + k)
        
        if (!deformFlag) { printf("Transform Start!!\n"); }
        deformFlag = true;

        for (int i = 0; i < pointRowNum; i++) {
            for (int j = 0; j < pointColNum; j++) {
                /* ポリゴンの形ごとに特殊な処理が必要な場合 */
                switch (SHAPE) {
                case BALL:
                    /* スパイクするように点を動かす */
                    if ((i + j + 1) % 2 == 0 && (i + 1) % 2 == 0) {

                        point[i][j] = prePoint[i][j] * scale;
                    }
                    break;
                case CUBE:
                    /* つぶれるように点を動かす */
                    point[i][j].x = prePoint[i][j].x * scale;
                    point[i][j].z = prePoint[i][j].z * scale;
                    point[i][j].y = prePoint[i][j].y / (scale);
                    break;
                case HEXAGON:

                    if (j % 2) {
                        point[i][j].x = prePoint[i][j].x * scale;
                        point[i][j].y = prePoint[i][j].y * scale;
                    }
                    else {
                        point[i][j].x = prePoint[i][j].x / scale;
                        point[i][j].y = prePoint[i][j].y / scale;
                    }
                    break;
                case SHADOWCUBE:
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
    
    if(pick > 4 && deformFlag) {
        printf("Transform Finished!!!\n");
        deformFlag = false;
        
    }
}
/***********************************************************
|  関数：easykaitenKakudai()
|  説明：ポリゴンを回転・拡大する関数（簡単モード）
|  引数：Hand hand
|  戻値：無し
***********************************************************/
void easyKaitenKakudai(Hand hand) {
    
    double pick = getPick(hand);

    if (!deformFlag && pick <= 4 || (rotateFlag && scaleFlag)) {
        if (!rotateFlag && !scaleFlag) { printf("Rotation and Scaling Start!!!\n"); }
        rotateFlag = true;
        scaleFlag = true;

        /* 拡大倍率を計算 */
        double scale = 1 + (pointer[1].y - prePointer[1].y) / 30;
        /* 移動量を計算 */
        double MoveX = pointer[1].x - prePointer[1].x;

        for (int i = 0; i < pointRowNum; i++) {
            for (int j = 0; j < pointColNum; j++) {

                /* 拡大 */
                point[i][j] = scale * prePoint[i][j];
                
                /* 回転 */
                ////* y軸周りに回転 *////
                double RotateY = (point[i][j].x == 0) ? PI / 2 * sign(point[i][j].z) : atan((point[i][j].z / abs(point[i][j].x)));    /* 頂点の位置（y軸周りの角度） */
                int XMark = (point[i][j].x < 0) ? -1 : 1;     /* ⅹの正負 */
                double YDistance = (double)pow(point[i][j].z * point[i][j].z + point[i][j].x * point[i][j].x, 0.5);     /* y軸までの距離 */
                RotateY -= 0.1 * XMark * MoveX ;
                /* 回転させる */
                point[i][j].z = YDistance * sin(RotateY);
                point[i][j].x = YDistance * cos(RotateY) * XMark;
            }
        }
    }
    if(pick > 4 && rotateFlag && scaleFlag) {
        rotateFlag = false;
        scaleFlag = false;
        printf("Rotation and Scaling Finished!!\n");
    }

}

/***********************************************************
|  関数：autoKaiten()
|  説明：ポリゴンを自動回転する関数
|  引数：無し
|  戻値：無し
***********************************************************/
void autoKaiten() {
    if (!deformFlag && !rotateFlag && !scaleFlag) {
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


                ////* y軸周りに回転 *////
                double RotateY = (point[i][j].x == 0) ? PI / 2 * sign(point[i][j].z) : atan((point[i][j].z / abs(point[i][j].x)));    /* 頂点の位置（y軸周りの角度） */
                int XMark = (point[i][j].x < 0) ? -1 : 1;     /* ⅹの正負 */
                double YDistance = (double)pow(point[i][j].z * point[i][j].z + point[i][j].x * point[i][j].x, 0.5);     /* y軸までの距離 */
                RotateY -= 0.003 * XMark;
                /* 回転させる */
                point[i][j].z = YDistance * sin(RotateY);
                point[i][j].x = YDistance * cos(RotateY) * XMark;


                ////* z軸周りに回転 *////
                double RotateZ = (point[i][j].y == 0) ? PI / 2 * sign(point[i][j].x) : atan((point[i][j].x / abs(point[i][j].y)));    /* 頂点の位置（z軸周りの角度） */
                int YMark = (point[i][j].y < 0) ? -1 : 1;     /* yの正負 */
                double ZDistance = (double)pow(point[i][j].x * point[i][j].x + point[i][j].y * point[i][j].y, 0.5);     /* z軸までの距離 */
                RotateZ -= 0.011 * YMark;
                /* 回転させる */
                point[i][j].x = ZDistance * sin(RotateZ);
                point[i][j].y = ZDistance * cos(RotateZ) * YMark;
            }
        }
    }
}

/***********************************************************
|  関数：getPick()
|  説明：人指し指と親指の距離を返す関数　またポインターの位置も更新する
|  引数：Hand hand
|  戻値：無し
***********************************************************/
double getPick(Hand hand) {
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
            posi0.y -= 20;
            posi0.z += 15;
        }
        else if (j == 1) {
            posi1 = currentPosition / 8;
            posi1.y -= 20;
            posi1.z += 15;
        }

        //個別の指の情報を出力する
        // printf("    finger[%d] (%6.1f,%6.1f,%6.1f)\n",
        //   j, currentPosition.x , currentPosition.y, currentPosition.z);
    }

    Center = (posi0 + posi1) / 2;
    pick = posi0.distanceTo(posi1);
    pointer[hand.isRight() ? 1: 0] = Center / 2;
    return pick;
}

/***********************************************************
|  関数：updateNearestPoint()
|  説明：ポインタに一番近い頂点を探す関数
|  引数：int* I        ポインタに一番近い頂点の配列の位置（row）
|  引数：int* J        ポインタに一番近い頂点の配列の位置（column）
|  戻値：無し
***********************************************************/
double updateNearestPoint(int* I, int* J) {

    double dist = 100;

    /* pointerに最も近い頂点を探す */
    Vector tmpNearestPoint = { 0,0,0 };
    for (int i = 0; i < pointRowNum; i++) {
        for (int j = 0; j < pointColNum; j++) {
            dist = point[i][j].distanceTo(pointer[0]);
            if (dist < tmpNearestPoint.distanceTo(pointer[0])) {
                tmpNearestPoint = point[i][j];
                *I = i;
                *J = j;
            }
        }
    }

    dist = pointer[0].distanceTo(tmpNearestPoint);
    nearestPoint = tmpNearestPoint;

    return dist;
}

/***********************************************************
|  関数：updatePrePoint()
|  説明：一個前のpoint・pointerを更新する関数
|  戻値：無し
***********************************************************/
void updatePrePoint() {
    /********************************************
    * 頂点を更新する前に保存 （更新中は保存しない）
    *******************************************/
    if (!deformFlag && !rotateFlag && !scaleFlag) {
        prePointer[0] = pointer[0];
        prePointer[1] = pointer[1];
        for (int i = 0; i < pointRowNum; i++) {
            for (int j = 0; j < pointColNum; j++) {
                prePoint[i][j] = point[i][j];
            }
        }
    }
}
