/* --------------------------------------------------------------------
 * gls02.c
 * OpenGL を使って立方体を回転させる
 * -------------------------------------------------------------------- */

 /* ヘッダファイル */
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "Leap.h"
#include "SampleListener.h"
#include "freeglut.h"
#include "initializePolygon.h"

using namespace std;
using namespace Leap;


/* グローバル変数 */
int   xBegin = 0;               /* マウスドラッグの始点X座標 */
int   yBegin = 0;               /* マウスドラッグの始点Y座標 */
int   PressButton = 0;          /* 現在押されているマウスボタン（1:左,2:中,3:右） */
float CameraAzimuth = 90.0;   /* カメラの位置（方位角） */
float CameraElevation = 0.0;    /* カメラの位置（仰角） */
float CameraDistance = 25.0;    /* カメラの位置（原点からの距離） */
float CameraX = 0.0;            /* カメラの位置（X座標） */
float CameraY = 10.0;            /* カメラの位置（Y座標） */
float CameraZ = 25.0;            /* カメラの位置（Z座標） */
float BoxRotate = 0.0;          /* 箱の回転角 */

Vector point[20][20] = {};      /* 頂点の座標 */
Vector prePoint[20][20] = {};   /* 以前の頂点の座標 */
int pointRowNum = 0;          /* 頂点の数（行） */
int pointColNum = 0;            /* 頂点の数（列） */

   
polygon shape;      /* polygonの形 */

Vector pointer[2] = { {100,100,100} ,{100,100,100} };   /* 変形させる際のポインターの位置 */
Vector prePointer[2] = { {100,100,100} ,{100,100,100} };        /* 1つ前のポインターの位置 */
Vector nearestPoint = { 100,100,100 };/* ポインターに最も近い頂点 */
//Vector rotateStart = { 0,0,0 };     /* 回転させる際の指のスタート位置 */
//Vector rotateNow = { 0,-999,0 };    /* 回転させる際の指の現在位置　 */

bool easyMode = true;       /* 簡単モード */

int deformFlag = -1;        /* 変形のフラグ*/
int rotateFlag = -1;      /* 回転のフラグ */
int scaleFlag = -1;       /* 拡大・縮小のフラグ */


unsigned char image[400][640][3];       /* 背景の画像 */

/***********************
ポリゴンの初期化用変数 
**************************/

/* 球 */
const int latitudeNUM = 16;         /* 緯度方向の分割数 */
const int longitudeNUM = 20;        /* 経度方向の分割数 */
const double radiusBall = 8;                    /* 球の半径 */

/* 立方体 */
const double length = 10;

/* 六角形 */
const double radiusHexagon = 6;




//必要だろうか。。。？
Controller controller;
SampleListener listener;
Screen screen;


/***********************************************************
|  関数：main()
|  説明：メイン関数
|  引数：int argc       実行時引数の数
|  引数：char** argv    実行時引数の内容（文字列配列）
|  戻値：int            0:正常終了
***********************************************************/
int main(int argc, char** argv)
{
    SampleListener listener;
    Controller controller;
    controller.addListener(listener);

    /* 初期化 */
    glutInit(&argc, argv);  /* OpenGL の初期化 */
    InitBall();
    myInit(argv[0]);        /* ウインドウ表示と描画設定の初期化 */

    /* イベント処理ループ */
    glutMainLoop();

    /* プログラム終了 */
    return(0);
}

float sign(float i) {			/* 符号を返す */
    if (i < 0) {
        return (float) - 1;
    }
    else {
        return (float) 1;
    }

}