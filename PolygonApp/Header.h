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

extern const int latitudeNUM;
extern const int longitudeNUM;
extern Vector point[13][19];
extern Vector prePoint[13][19];
extern const int R;
extern Vector pole[2];
extern Vector pointer;
extern Vector rotateStart;
extern Vector rotateEnd;

extern int movingFlag;
extern int rotatingFlag;
extern int scalingFlag;