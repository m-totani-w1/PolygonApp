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

using namespace std;
using namespace Leap;


/* グローバル変数 */
int   xBegin = 0;               /* マウスドラッグの始点X座標 */
int   yBegin = 0;               /* マウスドラッグの始点Y座標 */
int   PressButton = 0;          /* 現在押されているマウスボタン（1:左,2:中,3:右） */
float CameraAzimuth = 90.0;   /* カメラの位置（方位角） */
float CameraElevation = 0.0;    /* カメラの位置（仰角） */
float CameraDistance = 16.0;    /* カメラの位置（原点からの距離） */
float CameraX = 0.0;            /* カメラの位置（X座標） */
float CameraY = 0.0;            /* カメラの位置（Y座標） */
float CameraZ = 18.0;            /* カメラの位置（Z座標） */
float BoxRotate = 0.0;          /* 箱の回転角 */

const int latitudeNUM = 12;
const int longitudeNUM = 18;
const int R = 6;
Vector point[13][19] = {};
Vector prePoint[13][19] = {};
Vector pole[2] = {};
Vector pointer = { 100,100,100 };
Vector rotateStart = { 0,0,0 };
Vector rotateEnd = { 0,-999,0 };


int movingFlag = -1;
int rotatingFlag = -1;
int scalingFlag = -1;

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
    InitPoint();
    myInit(argv[0]);        /* ウインドウ表示と描画設定の初期化 */

    /* イベント処理ループ */
    glutMainLoop();

    /* プログラム終了 */
    return(0);
}