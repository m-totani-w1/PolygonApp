#include "initializePolygon.h"
#include "Header.h"
/*********************************************************
*
*   球
*
**********************************************************/

void InitBall() {
    /*以前の座標の消去*/
    clearPoint();

    shape = ball;
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

/*********************************************************
*
*   立方体
*
**********************************************************/


void InitCube() {
    /*以前の座標の消去*/
    clearPoint();

    shape = cube;
    /* 頂点の個数を更新*/
    pointRowNum = 2;
    pointColNum = 4;

    /**/
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

/*********************************************************
*
*   六角形
*
**********************************************************/


void InitHexagon() {
    /*以前の座標の消去*/
    clearPoint();

    shape = hexagon;
    /* 頂点の個数を更新*/
    pointRowNum = 1;
    pointColNum = 6;

    /**/
    for (int i = 0; i < 6; i++) {
        point[0][i].x = radiusHexagon * cos(i * (2 * PI / 6));
        point[0][i].y = radiusHexagon * sin(i * (2 * PI / 6));
        point[0][i].z = 0;
        printf("point0[][%d](x:%2.4f,y:%2.4f,z:%2.4f)\n", i, point[0][i].x, point[0][i].y, point[0][i].z);

    }
}

void clearPoint() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            point[i][j] = {0,-100,0};
        }
    }
}

