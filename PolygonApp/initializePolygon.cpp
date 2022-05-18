#include "initializePolygon.h"
#include "Header.h"
/*********************************************************
*
*   球
*
**********************************************************/

void InitBall() {
    /* 頂点の個数を更新*/
    pointRowNum = latitudeNUM;
    pointColNum = longitudeNUM;

    double theta = -PI / 2;
    double omega = 0;
    double Dtheta = -PI / latitudeNUM;
    double Domega = -2 * PI / longitudeNUM;

    /* 二つの極の座標 */
    theta = -PI / 2;
    omega = 0;
    point[0][0].x = R * cos(theta) * cos(omega);
    point[0][0].y = R * sin(theta);
    point[0][0].z = R * cos(theta) * sin(omega);
    printf("pole[0](x:%2.4f,y:%2.4f,z:%2.4f)\n", point[0][0].x, point[0][0].y, point[0][0].z);
    theta = PI / 2;
    omega = 0;
    point[latitudeNUM][0].x = R * cos(theta) * cos(omega);
    point[latitudeNUM][0].y = R * sin(theta);
    point[latitudeNUM][0].z = R * cos(theta) * sin(omega);
    printf("pole[1](x:%2.4f,y:%2.4f,z:%2.4f)\n", point[latitudeNUM][0].x, point[latitudeNUM][0].y, point[latitudeNUM][0].z);

    theta = -PI / 2;
    for (int i = 1; i < latitudeNUM; i++) {
        theta += Dtheta;
        omega = 0;
        for (int j = 0; j < longitudeNUM; j++) {
            printf("omega:%f  theta:%f\n", omega / PI, theta / PI);
            point[i][j].x = R * cos(theta) * cos(omega);
            point[i][j].y = R * sin(theta);
            point[i][j].z = R * cos(theta) * sin(omega);
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

}

/*********************************************************
*
*   立方体
*
**********************************************************/


void InitHexagon() {

}