#include "initializePolygon.h"
#include "CommonVariables.h"


/***********************************************************
|  関数：InitBall()
|  説明：初期化関数（ボール）
|  引数：無し
|  戻値：無し
***********************************************************/
void InitBall() {

    const int latitudeNUM = 16;         /* 緯度方向の分割数 */
    const int longitudeNUM = 20;        /* 経度方向の分割数 */
    const double radiusBall = 8;                    /* 球の半径 */

    /*以前の座標の消去*/
    clearPoint();

    SHAPE = BALL;
    /* 頂点の個数を更新*/
    pointRowNum = latitudeNUM+1;
    pointColNum = longitudeNUM;

    double theta = -PI / 2;
    double omega = 0;
    double Dtheta = PI / (latitudeNUM);
    double Domega = -2 * PI / longitudeNUM;

    /* 二つの極の座標 */
    theta = -PI / 2;
    omega = 0;
    point[0][0].x = radiusBall * cos(theta) * cos(omega);
    point[0][0].y = radiusBall * sin(theta);
    point[0][0].z = radiusBall * cos(theta) * sin(omega);
    printf("pole[0](x:%2.4f,y:%2.4f,z:%2.4f)\n", point[0][0].x, point[0][0].y, point[0][0].z);
    
    theta = PI / 2;
    omega = 0;
    point[latitudeNUM][0].x = radiusBall * cos(theta) * cos(omega);
    point[latitudeNUM][0].y = radiusBall * sin(theta);
    point[latitudeNUM][0].z = radiusBall * cos(theta) * sin(omega);
    printf("pole[1](x:%2.4f,y:%2.4f,z:%2.4f)\n", point[latitudeNUM][0].x, point[latitudeNUM][0].y, point[latitudeNUM][0].z);

    /* 極以外の座標 */
    theta = -PI / 2;
    for (int i = 1; i < latitudeNUM; i++) {
        theta += Dtheta;
        omega = 0;
        for (int j = 0; j < longitudeNUM; j++) {
            point[i][j].x = radiusBall * cos(theta) * cos(omega);
            point[i][j].y = radiusBall * sin(theta);
            point[i][j].z = radiusBall * cos(theta) * sin(omega);
            printf("omega:%f  theta:%f", omega / PI, theta / PI);
            printf("point[%d][%d](x:%2.4f,y:%2.4f,z:%2.4f)\n", i, j, point[i][j].x, point[i][j].y, point[i][j].z);
            omega += Domega;
        }

    }
}

/***********************************************************
|  関数：InitCube()
|  説明：初期化関数（立方体）
|  引数：無し
|  戻値：無し
***********************************************************/
void InitCube() {
    const double length = 10;

    /*以前の座標の消去*/
    clearPoint();

    SHAPE = CUBE;
    /* 頂点の個数を更新*/
    pointRowNum = 2;
    pointColNum = 4;

    /* 座標計算 */
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                point[i][j*2+k].x = length / 2 * ((j == 0) ? 1 : -1);
                point[i][j*2+k].y = length / 2 * ((i == 0) ? 1 : -1);
                point[i][j*2+k].z = length / 2 * ((k == 0) ? 1 : -1);
                printf("point[%d][%d](x:%2.4f,y:%2.4f,z:%2.4f)\n", i, j * 2 + k, point[i][j * 2 + k].x, point[i][j * 2 + k].y, point[i][j * 2 + k].z);
            }
        }

    }
}

/***********************************************************
|  関数：InitHexagon()
|  説明：初期化関数（六角形）
|  引数：無し
|  戻値：無し
***********************************************************/
void InitHexagon() {
    const double radiusHexagon = 6;

    /*以前の座標の消去*/
    clearPoint();

    SHAPE = HEXAGON;
    /* 頂点の個数を更新*/
    pointRowNum = 1;
    pointColNum = 6;

    /*　座標計算　*/
    for (int i = 0; i < 6; i++) {
        point[0][i].x = radiusHexagon * cos(i * (2 * PI / 6));
        point[0][i].y = radiusHexagon * sin(i * (2 * PI / 6));
        point[0][i].z = 0;
        printf("point0[][%d](x:%2.4f,y:%2.4f,z:%2.4f)\n", i, point[0][i].x, point[0][i].y, point[0][i].z);

    }
}

/***********************************************************
|  関数：InitShadowCube()
|  説明：初期化関数（陰影立方体）
|  引数：無し
|  戻値：無し
***********************************************************/
void InitShadowCube() {
    const double length = 10;
    double lengthS = 10;
    Vector RotateAxis = { 1,0,1 };

    clearPoint();

    SHAPE = SHADOWCUBE;

    /* 頂点の個数を更新*/
    pointRowNum = 2;
    pointColNum = 4;

    /*　座標計算　*/
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            point[i][j].y = length / 2 * ((i == 0) ? 1 : -1);
            point[i][j].x = length / 2 * ((j == 0 || j == 1) ? 1 : -1);
            point[i][j].z = length / 2 * ((j == 0 || j == 3) ? 1 : -1);
            printf("point[%d][%d](x:%2.4f,y:%2.4f,z:%2.4f)\n", i, j, point[i][j].x, point[i][j].y, point[i][j].z);
        }
    }

}

/***********************************************************
|  関数：clearPoint()
|  説明：頂点の状態を削除する関数
|  引数：無し
|  戻値：無し
***********************************************************/
void clearPoint() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            point[i][j] = {0,-100,0};
        }
    }
}

