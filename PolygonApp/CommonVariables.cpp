#include "Leap.h"
#include "CommonVariables.h"

using namespace Leap;

/****************************
*グローバル変数 
*****************************/

/* ポインターの座標 */
Vector pointer[2] = { {100,100,100} ,{100,100,100} };   /* 変形させる際のポインターの位置(左手なら０、右手なら１) */
Vector prePointer[2] = { {100,100,100} ,{100,100,100} };        /* 1つ前のポインターの位置 */
Vector nearestPoint = { 100,100,100 };/* ポインターに最も近い頂点 */

/* アプリのモード　*/
bool easyMode = true;       /* 簡単モード */

/* なにを操作中か*/
bool deformFlag = false;        /* 変形してるかどうか*/
bool rotateFlag = false;      /* 回転してるかどうか*/
bool scaleFlag = false;       /* 拡大・縮小してるかどうか */

/* ポリゴンの頂点 */
Vector point[20][20] = {};      /* 頂点の座標 */
Vector prePoint[20][20] = {};   /* 以前の頂点の座標 */
int pointRowNum = 0;          /* 頂点の数（行） */
int pointColNum = 0;            /* 頂点の数（列） */
polygon shape;      /* polygonの形 */