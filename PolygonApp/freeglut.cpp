
#include <math.h>
#include<stdio.h>
#include<stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Leap.h"
#include "SampleListener.h"
#include "initializePolygon.h"
#include "Convenient.h"

/************************
* glutの初期値
*************************/
/* 空間 */
int   xBegin = 0;               /* マウスドラッグの始点X座標 */
int   yBegin = 0;               /* マウスドラッグの始点Y座標 */
int   PressButton = 0;          /* 現在押されているマウスボタン（1:左,2:中,3:右） */
float CameraAzimuth = 90.0;   /* カメラの位置（方位角） */
float CameraElevation = -10.0;    /* カメラの位置（仰角） */
float CameraDistance = 25.0;    /* カメラの位置（原点からの距離） */
float CameraX = 0.0;            /* カメラの位置（X座標） */
float CameraY = 10.0;            /* カメラの位置（Y座標） */
float CameraZ = 25.0;            /* カメラの位置（Z座標） */

/* 地面 */
double ground_max_x = 80.0;     
double ground_max_y = 30.0;
double ground_max_z = 100.0;
bool flag_startedOperation = false;      /* 操作（初期化・切替）を始めた。 */
int motionRange = -1;              /*  */

/* 背景 */
bool colorIsChanging[] = { true ,false,false};
int changingColor = 0;
GLfloat background[] = { 0, 0, 0, 1.0 };//背景色


/* 色 */
GLfloat green[] = { 0.2, 0.8, 0.2, 1.0 };       //緑色
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };        //青色
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };         //赤色
GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };      //黄色

GLfloat gray[] = { 0.2, 0.2, 0.2, 1.0 };        //灰色
GLfloat white2[] = { 0.4, 0.4, 0.4, 1.0 };      //白色2
GLfloat white1[] = { 0.6, 0.6, 0.6, 1.0 };      //白色1
GLfloat white[] = { 0.8, 0.8, 0.8, 1.0 };       //白色
GLfloat realWhite[] = { 1, 1, 1, 1 };           //真っ白
GLfloat black[] = { 0, 0, 0, 1.0 };             //黒色


/***********************************************************
|  関数：drawGround()
|  説明：地面を描画する関数
|  引数：無し
|  戻値：無し
***********************************************************/
void drawGround(void) {

    /*　地面　*/

    //陰影ON-----------------------------
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);//光源0を利用
    //-----------------------------------  

    glBegin(GL_QUADS);
    for (double lx = -ground_max_x; lx <= ground_max_x; lx += 5) {
        for (double lz = -ground_max_z; lz <= 30; lz += 5) {
            if (easyMode) { 
                if (motionRange == (int)pow(pow(lx,2) + pow(lz,2),0.5) / 5) {
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, realWhite);
                }
                else if (motionRange-1 == (int)pow(pow(lx, 2) + pow(lz, 2), 0.5) / 5) {
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
                }
                else if (motionRange-2 == (int)pow(pow(lx, 2) + pow(lz, 2), 0.5) / 5) {
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, white1);
                }
                else if (motionRange-3 == (int)pow(pow(lx, 2) + pow(lz, 2), 0.5) / 5) {
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, white2);
                }
                else if ((int)(lx + ((int)lz) % 2 + 1) % 2) {                    
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);                    
                }
                else {
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
                }
            }
            else {
                if (motionRange == (int)pow(pow(lx, 2) + pow(lz, 2), 0.5) / 5) {
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, realWhite);
                }
                else if (motionRange - 1 == (int)pow(pow(lx, 2) + pow(lz, 2), 0.5) / 5) {
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
                }
                else if (motionRange - 2 == (int)pow(pow(lx, 2) + pow(lz, 2), 0.5) / 5) {
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, white1);
                }
                else if (motionRange - 3 == (int)pow(pow(lx, 2) + pow(lz, 2), 0.5) / 5) {
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, white2);
                }
                else if ((int)(lx + ((int)lz) % 2 + 1) % 2) {
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
                }
                else {
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
                }
            }
            glNormal3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
            glVertex3d(lx, -15.1, lz);
            glVertex3d(lx, -15.1, lz+5);
            glVertex3d(lx+5, -15.1, lz+5);
            glVertex3d(lx + 5, -15.1, lz);   
        } 
    }
    glEnd();
    
    //陰影OFF-----------------------------
    glDisable(GL_LIGHTING);
    //-----------------------------------
    

    
    /*地面の線*/

    glColor3d(0, 0, 0);  // 大地の線の色
    glBegin(GL_LINES);
    for (double lz = -ground_max_z; lz <= ground_max_z; lz += 5) {
        glVertex3d(-ground_max_x,-15,lz);
        glVertex3d(ground_max_x,-15,lz);  
    }
    for (double lx = -ground_max_x; lx <= ground_max_x; lx += 5) {
        glVertex3d(lx,-15, ground_max_z);
        glVertex3d(lx,-15, -ground_max_z);
    }
    glEnd();
    
}

/***********************************************************
|  関数：drawBackGround()
|  説明：背景を描画する関数
|  引数：無し
|  戻値：無し
***********************************************************/
void drawBackGround(void) {

    /* 背景 */
    
    glBegin(GL_POLYGON);
    glColor3d(0.8, 0.8, 0.8);  // 壁の色(白)
    glVertex3d(ground_max_x + 60, ground_max_y, -ground_max_z - 0.1);

    glColor3d(background[0], background[1], background[2]);
    glVertex3d(ground_max_x + 60, -15, -ground_max_z - 0.1);

    glColor3d(background[0], background[1], background[2]);
    glVertex3d(-ground_max_x - 60, -15, -ground_max_z - 0.1);

    glColor3d(0.8, 0.8, 0.8);  // 壁の色(白)
    glVertex3d(-ground_max_x - 60, ground_max_y, -ground_max_z - 0.1);
    glEnd();


    /*　背景の線　*/

    //for (double ly = -15; ly <= ground_max_y*2; ly += 5.0) {
    //    glVertex3d(-ground_max_x, ly, -ground_max_z);
    //    glVertex3d(ground_max_x, ly, -ground_max_z);
    //}
    //for (double lx = -ground_max_x; lx <= ground_max_x; lx += 5.0) {
    //    glVertex3d(lx, -15, -ground_max_z);
    //    glVertex3d(lx, ground_max_y*2, -ground_max_z);
    //}

}

/***********************************************************
|  関数：drawPointer()
|  説明：ポインター・ポインター付近の頂点を描画する関数
|  引数：無し
|  戻値：無し
***********************************************************/
void drawPointer(void) {
    

    //陰影ON-----------------------------
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);//光源0を利用
    //-----------------------------------

    /* ポインターの表示 */
    if (deformFlag ) {

        glTranslatef(pointer[0].x, pointer[0].y, pointer[0].z);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
        glutSolidSphere(0.3, 16, 16);
        glTranslatef(-pointer[0].x, -pointer[0].y, -pointer[0].z);

        glTranslatef(pointer[1].x, pointer[1].y, pointer[1].z);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
        glutSolidSphere(0.3, 16, 16);
        glTranslatef(-pointer[1].x, -pointer[1].y, -pointer[1].z);
    }
    else if (rotateFlag && scaleFlag) {

        glTranslatef(pointer[0].x, pointer[0].y, pointer[0].z);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
        glutSolidSphere(0.3, 16, 16);
        glTranslatef(-pointer[0].x, -pointer[0].y, -pointer[0].z);

        glTranslatef(pointer[1].x, pointer[1].y, pointer[1].z);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
        glutSolidSphere(0.3, 16, 16);
        glTranslatef(-pointer[1].x, -pointer[1].y, -pointer[1].z);
    }
    else if (rotateFlag) {

        glTranslatef(pointer[0].x, pointer[0].y, pointer[0].z);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
        glutSolidSphere(0.3, 16, 16);
        glTranslatef(-pointer[0].x, -pointer[0].y, -pointer[0].z);

        glTranslatef(pointer[1].x, pointer[1].y, pointer[1].z);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
        glutSolidSphere(0.3, 16, 16);
        glTranslatef(-pointer[1].x, -pointer[1].y, -pointer[1].z);
    }
    else if (scaleFlag) {

        glTranslatef(pointer[0].x, pointer[0].y, pointer[0].z);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
        glutSolidSphere(0.3, 16, 16);
        glTranslatef(-pointer[0].x, -pointer[0].y, -pointer[0].z);

        glTranslatef(pointer[1].x, pointer[1].y, pointer[1].z);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
        glutSolidSphere(0.3, 16, 16);
        glTranslatef(-pointer[1].x, -pointer[1].y, -pointer[1].z);
    }
    else {
        glTranslatef(pointer[0].x, pointer[0].y, pointer[0].z);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
        glutSolidSphere(0.3, 16, 16);
        glTranslatef(-pointer[0].x, -pointer[0].y, -pointer[0].z);

        glTranslatef(pointer[1].x, pointer[1].y, pointer[1].z);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
        glutSolidSphere(0.3, 16, 16);
        glTranslatef(-pointer[1].x, -pointer[1].y, -pointer[1].z);
    }

    /* ポインターと近い頂点の表示 */

    double dist = nearestPoint.distanceTo(pointer[0]);
    if (dist > 0.1) {
        if (dist < 2) {
            glTranslatef(nearestPoint.x, nearestPoint.y, nearestPoint.z);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
            glutSolidSphere(0.5, 16, 16);
            glTranslatef(-nearestPoint.x, -nearestPoint.y, -nearestPoint.z);
        }
        else {
            glTranslatef(nearestPoint.x, nearestPoint.y, nearestPoint.z);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
            glutSolidSphere(0.5, 16, 16);
            glTranslatef(-nearestPoint.x, -nearestPoint.y, -nearestPoint.z);
        }
    }

    /* ポインターと近い頂点を結ぶ線 */
    if ((!easyMode) && dist < 2) {
        glLineWidth((GLfloat)5);
        glColor3f(0.8, 0.2, 0.2);
        glBegin(GL_LINE_LOOP);
        glVertex3f(pointer[0].x, pointer[0].y, pointer[0].z);
        glVertex3f(nearestPoint.x, nearestPoint.y, nearestPoint.z);
        glEnd();
        glLineWidth((GLfloat)1);
    }


    //陰影OFF-----------------------------
    glDisable(GL_LIGHTING);
    //-----------------------------------
}

/***********************************************************
|  関数：drawPolygon()
|  説明：ポリゴンを描画する関数
|  引数：無し
|  戻値：無し
***********************************************************/
void drawPolygon(void) {
    /* ポリゴンの描画  */

    /* 描画用変数 */
    GLfloat color[4] = { 0,0,0,1 };
    Vector normal = { 0,0,1 };
    Vector normal2 = { 0,0,1 };
    Vector normal3 = { 0,0,1 };
    Vector normal4 = { 0,0,1 };
    GLfloat shadowCube_color[] = { 0.1, 0.1, 0.6, 1.0 };

    switch (SHAPE)
    {
    case BALL:
        //陰影ON-----------------------------
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);//光源0を利用
        //-----------------------------------

        glBegin(GL_QUADS);

        //極の周辺以外の描画
        for (int i = 1; i < pointRowNum - 2; i++) {   //0とpointRowNum-1は極なので、それ以外。

            for (int j = 0; j < pointColNum; j++) {

                glMaterialfv(GL_FRONT, GL_DIFFUSE, green);

                normal = (point[i][(j + 1) % pointColNum] - point[i][j]).cross(point[i + 1][j] - point[i][j]);
                normal2 = (point[i + 1][(j + 1) % pointColNum] - point[i][(j + 1) % pointColNum]).cross(point[i][j] - point[i][(j + 1) % pointColNum]);
                normal3 = (point[i + 1][j] - point[i + 1][(j + 1) % pointColNum]).cross(point[i][(j + 1) % pointColNum] - point[i + 1][(j + 1) % pointColNum]);
                normal4 = (point[i][j] - point[i + 1][j]).cross(point[i + 1][(j + 1) % pointColNum] - point[i + 1][j]);
                normal += normal2 + normal3 + normal4;
                normal = normal / normal.magnitude();
                glNormal3f((GLfloat)normal.x, (GLfloat)normal.y, (GLfloat)normal.z);

                glVertex3f(point[i][j].x, point[i][j].y, point[i][j].z);
                glVertex3f(point[i + 1][j].x, point[i + 1][j].y, point[i + 1][j].z);
                glVertex3f(point[i + 1][(j + 1) % pointColNum].x, point[i + 1][(j + 1) % pointColNum].y, point[i + 1][(j + 1) % pointColNum].z);
                glVertex3f(point[i][(j + 1) % pointColNum].x, point[i][(j + 1) % pointColNum].y, point[i][(j + 1) % pointColNum].z);
            }
        }

        //極の周辺の描画
        for (int i = 0; i < pointColNum; i++) {

            glMaterialfv(GL_FRONT, GL_DIFFUSE, green);


            normal = (point[0][0] - point[1][i]).cross(point[1][(i + 1) % pointColNum] - point[1][i]);
            normal = normal / normal.magnitude();
            glNormal3f((GLfloat)normal.x, (GLfloat)normal.y, (GLfloat)normal.z);

            glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
            glVertex3f(point[1][i].x, point[1][i].y, point[1][i].z);
            glVertex3f(point[1][(i + 1) % pointColNum].x, point[1][(i + 1) % pointColNum].y, point[1][(i + 1) % pointColNum].z);
            glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);


            normal = (point[pointRowNum - 2][(i + 1) % pointColNum] - point[pointRowNum - 2][i]).cross(point[pointRowNum - 1][0] - point[pointRowNum - 2][i]);
            normal = normal / normal.magnitude();
            glNormal3f((GLfloat)normal.x, (GLfloat)normal.y, (GLfloat)normal.z);

            glVertex3f(point[pointRowNum - 1][0].x, point[pointRowNum - 1][0].y, point[pointRowNum - 1][0].z);
            glVertex3f(point[pointRowNum - 1][0].x, point[pointRowNum - 1][0].y, point[pointRowNum - 1][0].z);
            glVertex3f(point[pointRowNum - 2][i].x, point[pointRowNum - 2][i].y, point[pointRowNum - 2][i].z);
            glVertex3f(point[pointRowNum - 2][(i + 1) % pointColNum].x, point[pointRowNum - 2][(i + 1) % pointColNum].y, point[pointRowNum - 2][(i + 1) % pointColNum].z);


        }

        glEnd();
        //陰影OFF-----------------------------
        glDisable(GL_LIGHTING);
        //-----------------------------------
        break;
    case CUBE:

        glBegin(GL_QUADS);

        /*　上面　*/
        glColor3f(0, 0, 0.8);
        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);
        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);

        /*　下面　*/
        glColor3f(0, 0.8, 0.8);
        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);

        /*　正面　*/
        glColor3f(0.8, 0.5, 0.5);
        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);
        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);

        /*　背面　*/
        glColor3f(0.8, 0.8, 0);
        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);

        /*　右側面　*/
        glColor3f(0.8, 0, 0);
        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);
        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);
        /*　左側面　*/
        glColor3f(0, 0.8, 0);
        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);

        glEnd();
        break;

    case HEXAGON:

        

        for (int i = 0; i < 6; i++) {
            glTranslatef(point[0][i].x, point[0][i].y, point[0][i].z);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
            glutSolidSphere(0.4,16 ,16 );
            glTranslatef(-point[0][i].x, -point[0][i].y, -point[0][i].z);
        }
        glLineWidth(15);
        glBegin(GL_LINE_LOOP);
        glColor3f(0.1, 0.8, 0.1);
        for (int i = 0; i < 6; i++) {
            glVertex3f(point[0][i].x, point[0][i].y, point[0][i].z);
        }
        glEnd();
        glLineWidth(1);

        break;
    case SHADOWCUBE:
        //陰影ON-----------------------------
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);//光源0を利用
        //-----------------------------------

        glBegin(GL_QUADS);

        /*　上面　*/
        /* 垂線の設定 */
        normal = (point[0][0] - point[0][1]).cross(point[0][1] - point[0][2]);
        normal = normal / normal.magnitude();
        glNormal3f((GLfloat)normal.x, (GLfloat)normal.y, (GLfloat)normal.z);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
        /* 面描画 */
        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);
        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);

        /*　下面　*/
        /* 垂線の設定 */
        normal = (point[1][3] - point[1][2]).cross(point[1][2] - point[1][1]);
        normal = normal / normal.magnitude();
        glNormal3f((GLfloat)normal.x, (GLfloat)normal.y, (GLfloat)normal.z);
        /* 面描画 */
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);
        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);

        /*　正面　*/
        /* 垂線の設定 */
        normal = (point[0][0] - point[0][3]).cross(point[0][3] - point[1][3]);
        normal = normal / normal.magnitude();
        glNormal3f((GLfloat)normal.x, (GLfloat)normal.y, (GLfloat)normal.z);
        /* 面描画 */
        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);

        /*　背面　*/
        /* 垂線の設定 */
        normal = (point[1][1] - point[1][2]).cross(point[1][2] - point[0][2]);
        normal = normal / normal.magnitude();
        glNormal3f((GLfloat)normal.x, (GLfloat)normal.y, (GLfloat)normal.z);
        /* 面描画 */
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);
        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);
        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);

        /*　右側面　*/
        /* 垂線の設定 */
        normal = (point[0][0] - point[1][0]).cross(point[1][0] - point[1][1]);
        normal = normal / normal.magnitude();
        glNormal3f((GLfloat)normal.x, (GLfloat)normal.y, (GLfloat)normal.z);
        /* 面描画 */
        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);
        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);

        /*　左側面　*/
        /* 垂線の設定 */
        normal = (point[0][2] - point[1][2]).cross(point[1][2] - point[1][3]);
        normal = normal / normal.magnitude();
        glNormal3f((GLfloat)normal.x, (GLfloat)normal.y, (GLfloat)normal.z);
        /* 面描画 */
        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);

        glEnd();


        //陰影OFF-----------------------------
        glDisable(GL_LIGHTING);
        //-----------------------------------
        break;
    default:
        break;
    }

}

/**********************************************************
| 関数：display()
| 説明：「１枚の」グラフィック描画イベント処理
| 引数：なし
| 戻値：なし
* **********************************************************/
void display(void)
{
    /* 初期化 */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   /* 画面を消去 */
    glMatrixMode(GL_MODELVIEW);             /* 幾何（描画位置など設定する）モード */
    glLoadIdentity();                       /* 幾何を初期化する */

    /* 視点の設定 */
    gluLookAt(CameraX, CameraY, CameraZ,  /* カメラの位置 */
        0.0, 0.0, 0.0,  /* 注視点の位置 */
        0.0, 1.0, 0.0); /* カメラ上方向のベクトル */

    
    /* 地面の描画　*/
    drawGround();
    /* 背景の描画 */
    drawBackGround();
    /* ポインターの描画 */
    drawPointer();
    /* ポリゴンの描画 */
    drawPolygon();

    
    /* 上記で描画されたCGをモニターに出力 */
    glutSwapBuffers();
}

/***********************************************************
|  関数：timer(int timerID)
|  説明：タイマー（設定時間経過）イベント処理
|  引数：int timerID    イベントが発生したタイマーの識別ID
|  戻値：なし
***********************************************************/
void timer(int timerID)
{
    /* 次のタイマーを15ミリ秒後に設定 */
    glutTimerFunc(20, timer, 0);

    /* 背景の色を変える */
    if (colorIsChanging[changingColor]) {
        if (background[changingColor] < 0.1) {
            background[changingColor] += 0.001;
        }
        else {
            colorIsChanging[changingColor] = false;
        }
    }
    else {
        if (background[changingColor] > 0.01) {
            background[changingColor] -= 0.001;
        }
        else {
            changingColor = (changingColor + 1) % 3;
            colorIsChanging[changingColor] = true;
        }
    }

    /* 地面の変化を止めるかどうか */
    if (flag_startedOperation)motionRange++;
    if (flag_startedOperation && motionRange >= 5 + (int)pow(pow(ground_max_x, 2) + pow(ground_max_z, 2), 0.5) / 5) {
        motionRange=-1; 
        flag_startedOperation = false;
    }


    /* 描画要求（直後に display() 関数が呼ばれる） */
    glutPostRedisplay();
}

/***********************************************************
|  関数：keyboard()
|  説明：キーボードが押された時のイベント処理
|  引数：unsigned char key  押されたキーの文字コード
|  引数：int x              キーが押されたときのマウスポインタのX座標
|  引数：int y              キーが押されたときのマウスポインタのY座標
|  戻値：なし
***********************************************************/
void keyboard(unsigned char key, int x, int y)
{
    /* キーボード処理 */
    switch (key)
    {
    case 'm':/* 操作モード変更 */
        if (easyMode) {
            easyMode = false;
            nearestPoint = { 100,100,100 };
        }
        else {
            easyMode = true;
            nearestPoint = { 100,100,100 };
        }
        break;
    case 'i':
        shokika();      /* 初期化 */
        flag_startedOperation = true;
        break;
    case 's':
        kirikae();      /* 切替 */
        flag_startedOperation = true;
        break;
    case 'h':
        InitHexagon();
        flag_startedOperation = true;
        break;
    case 'c':
        InitCube();
        flag_startedOperation = true;
        break;
    case 'b':
        InitBall();
        flag_startedOperation = true;
        break;
    case 'q':
        exit(0);      /* プログラム終了 */
        break;
    case 'r':
        autoKaiten();      /* プログラム終了 */
        break;
    }
    Sleep(10);

    /* 描画要求（直後に display() 関数が呼ばれる） */
    glutPostRedisplay();

    /* コンパイル時の警告対策（定義された変数を使わないと警告になるので） */
    x = y = 0;
}

/***********************************************************
|  関数：mouseButton()
|  説明：マウスのボタン操作時のイベント処理
|  引数：int button     操作したマウスボタンの番号
|  引数：int state      操作の種類 GLUT_DOWN（押す）かGLUT_UP（離す）
|  引数：int x          キーが押されたときのマウスポインタのX座標
|  引数：int y          キーが押されたときのマウスポインタのY座標
|  戻値：なし
***********************************************************/
void mouseButton(int button, int state, int x, int y)
{
    /* マウスが押された時 */
    if (state == GLUT_DOWN)
    {
        switch (button)
        {
        case GLUT_LEFT_BUTTON:  /* マウス左ボタンを押した時の処理 */
            PressButton = button;
            //shokika();      /* 初期化 */
            break;

        case GLUT_MIDDLE_BUTTON:/* マウス中ボタンを押した時の処理 */
            break;

        case GLUT_RIGHT_BUTTON: /* マウス右ボタンを押した時の処理 */
            PressButton = button;
            //kirikae(1);      /* 切替 */
            break;
        }

        /* マウスボタンを押した瞬間の始点座標を記録する */
        xBegin = x;
        yBegin = y;
    }
}

/***********************************************************
|  関数：mouseDrag()
|  説明：画面上でマウスがドラッグされた時のイベント処理
|  引数：int x          現在のマウスポインタのX座標
|  引数：int y          現在のマウスポインタのY座標
|  戻値：なし
***********************************************************/
void mouseDrag(int x, int y)
{
    /* マウスボタンが押されてからの移動量を計算する */
    int xMove = x - xBegin;
    int yMove = y - yBegin;

    switch (PressButton)
    {
        /* マウス左ボタンドラッグ時の処理 */
        /* 視点を方位角，仰角に対して回転させる */
    case GLUT_LEFT_BUTTON:
        CameraAzimuth += (float)xMove / 2.0;
        CameraElevation += (float)yMove / 2.0;
        if (CameraElevation > 90.0) CameraElevation = 90.0;
        if (CameraElevation < -90.0) CameraElevation = -90.0;
        break;

        /* マウス右ボタンドラッグ時の処理 */
        /* 視点を奥行き方向に前後に移動させる */
    case GLUT_RIGHT_BUTTON:
        CameraDistance += (float)yMove / 40.0;
        break;
    }
    CameraX = CameraDistance * cos(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
    CameraY = CameraDistance * sin(CameraElevation * RAD);
    CameraZ = CameraDistance * sin(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
    printf("Camera AZ:%.1f, EL:%.1f, dist:%.1f, x,y,z= %.1f, %.1f, %.1f\n",
        CameraAzimuth, CameraElevation, CameraDistance, CameraX, CameraY, CameraZ);

    /* 現在のマウスポインタの座標を次の始点用に記録する */
    xBegin = x;
    yBegin = y;

    /* 描画要求（直後に display() 関数が呼ばれる） */
    glutPostRedisplay();
}

/***********************************************************
|  関数：myInit()
|  説明：ウインドウ表示と描画設定の初期化
|  引数：char *windowTitle      ウインドウのタイトルバーに表示する文字列
|  戻値：なし
***********************************************************/
void myInit(char* windowTitle)
{
    /* ウインドウのサイズ */
    int winWidth = 1260;
    int winHeight = 640;
    /* ウインドウの縦横の比を計算 */
    float aspect = (float)winWidth / (float)winHeight;

    /* OpenGLウインドウ作成までの初期化 */
    glutInitWindowPosition(0, 0);                   /* ウインドウ表示位置 */
    glutInitWindowSize(winWidth, winHeight);        /* ウインドウサイズ */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);   /* 描画モード */
    glutCreateWindow(windowTitle);                  /* ウインドウの表示 */
    glClearColor(0, 0, 0, 1.0);              /* 画面消去色の設定 */

    //光源の設定--------------------------------------
    GLfloat light_position0[] = { CameraX+15, CameraY + 10, CameraZ, 1.0 }; //光源0の座標
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0); //光源0を
    GLfloat ambient[] = { 0.7, 0.7, 0.9, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    /* イベント発生時に呼び出す関数の登録 */
    glutKeyboardFunc(keyboard);     /* キーボードを押した時 */
    glutMouseFunc(mouseButton);  /* マウスボタンを押した時*/
    glutMotionFunc(mouseDrag);    /* マウスドラッグした時 */
    glutDisplayFunc(display);      /* 画面表示 */
    glutTimerFunc(15, timer, 0);    /* タイマーを15ミリ秒後に設定 */

    /* CG描画設定 */
    glMatrixMode(GL_PROJECTION);    /* 透視投影(遠近投影法)設定モードに切り替え */
    glLoadIdentity();               /* 透視投影行列を初期化 */
    gluPerspective(50.0, aspect, 1.0, 150.0);    /* 透視投影行列の設定 */
      /* 視野角45度, 縦横比 aspect，描画前面までの奥行 1.0，描画背面までの奥行 20.0 */

    glEnable(GL_DEPTH_TEST);        /* 隠面消去を有効にする */

}

