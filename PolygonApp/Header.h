#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "Leap.h"

#ifndef Header_H
#define Header_H

#define M_PI 3.14159265358979323846  /* pi */
#define RAD (M_PI / 180.0)

#endif

using namespace Leap;

/* グローバル変数 */
extern int   xBegin;               /* マウスドラッグの始点X座標 */
extern int   yBegin;               /* マウスドラッグの始点Y座標 */
extern int   PressButton;          /* 現在押されているマウスボタン（1:左,2:中,3:右） */
extern float CameraAzimuth;   /* カメラの位置（方位角） */
extern float CameraElevation;    /* カメラの位置（仰角） */
extern float CameraDistance;    /* カメラの位置（原点からの距離） */
extern float CameraX;            /* カメラの位置（X座標） */
extern float CameraY;            /* カメラの位置（Y座標） */
extern float CameraZ;            /* カメラの位置（Z座標） */
extern float BoxRotate;          /* 箱の回転角 */

extern Vector point[20][20];      /* 頂点の座標 */
extern Vector prePoint[20][20];   /* 以前の頂点の座標 */
extern int pointRowNum;          /* 頂点の数（行） */
extern int pointColNum;            /* 頂点の数（列） */
enum polygon { ball, cube, hexagon };
extern polygon shape;      /* polygonの形 */

extern Vector pointer[2];   /* 変形させる際のポインターの位置 */
extern Vector prePointer[2];        /* 1つ前のポインターの位置 */
extern Vector nearestPoint;	/* ポインターに最も近い頂点 */
//extern Vector rotateStart;     /* 回転させる際の指のスタート位置 */
//extern Vector rotateNow;    /* 回転させる際の指の現在位置　 */

extern bool easyMode;	/* 簡単モード */

extern int deformFlag;        /* 変形のフラグ*/
extern int rotateFlag;      /* 回転のフラグ */
extern int scaleFlag;       /* 拡大・縮小のフラグ */


extern unsigned char image[400][640][3];       /* 背景の画像 */

/***********************
ポリゴンの初期化用変数
**************************/

/* 球 */
extern const int latitudeNUM;         /* 緯度方向の分割数 */
extern const int longitudeNUM;        /* 経度方向の分割数 */
extern const double radiusBall;                    /* 球の半径 */

/* 立方体 */
extern const double length;

/* 六角形 */
extern const double radiusHexagon;                    /* 六角形の半径 */

/*************************
* 便利な関数
**************************/
float sign(float i);			/* 符号を返す */