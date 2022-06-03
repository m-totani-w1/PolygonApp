
#include "Leap.h"
#include <stdio.h>

using namespace Leap;

/* グローバル変数 */

extern Vector point[20][20];      /* 頂点の座標 */
extern Vector prePoint[20][20];   /* 以前の頂点の座標 */
extern int pointRowNum;          /* 頂点の数（行） */
extern int pointColNum;            /* 頂点の数（列） */
enum polygon { BALL, CUBE, HEXAGON, SHADOWCUBE };
extern polygon SHAPE;      /* polygonの形 */

extern Vector pointer[2];   /* 変形させる際のポインターの位置 */
extern Vector prePointer[2];        /* 1つ前のポインターの位置 */
extern Vector nearestPoint;	/* ポインターに最も近い頂点 */

extern bool easyMode;	/* 簡単モード */

extern bool deformFlag;        /* 変形してるかどうか*/
extern bool rotateFlag;      /* 回転してるかどうか */
extern bool scaleFlag;       /* 拡大・縮小してるかどうかs */
