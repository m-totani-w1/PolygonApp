
#include <math.h>
#include<stdio.h>
#include<stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Header.h"
#include "Leap.h"
#include "SampleListener.h"
#include "initializePolygon.h"


/***************************************************
* 画像の読込
***************************************************/


int ppm_read(const char* filename, unsigned char* pimage) {
    FILE* fp;
    if ((ERROR != fopen_s(&fp,filename, "rb"))) {
        printf("ERROR:%s\n", filename);
        exit(-1);
    }
    fscanf_s(fp, "P6\n640 480\n255\n");
    fread(pimage, sizeof(char), 640 * 400 * 3, fp);
    fclose(fp);
    return 0;
}
//----------------------------------------------------
// 大地の描画
//----------------------------------------------------
void Ground(void) {
    double ground_max_x = 60.0;
    double ground_max_y = 30.0;
    double ground_max_z = 70.0;


    glBegin(GL_QUADS);
    for (double lx = -ground_max_x; lx <= ground_max_x; lx += 5) {
        for (double lz = -ground_max_z; lz <= ground_max_z; lz += 5) {
            
            if (easyMode) {
                if ((int)(lx + ((int)lz) % 2 + 1) % 2) {
                    glColor3d(0.7, 0.7, 0.7);  // 大地の色(1)
                }
                else {
                    glColor3d(0.4, 0.4, 0.4);  // 大地の色(2)
                }
            }
            else {
                if ((int)(lx + ((int)lz) % 2 + 1) % 2) {
                    glColor3d(0.7, 0.7, 0.9);  // 大地の色(1)
                }
                else {
                    glColor3d(0.4, 0.4, 0.5);  // 大地の色(2)
                }
            }
            //glColor3d(1, 1, 1);  // 大地の色(白色)
            //glColor3d(0.52, 0.29, 0.17);  // 大地の色(茶色)
            //glColor3d(0.645, 0.845, 0.268);  // 大地の色(草色)
            
            glVertex3d(lx, -15.1, lz);
            glVertex3d(lx, -15.1, lz+5);
            glVertex3d(lx+5, -15.1, lz+5);
            glVertex3d(lx + 5, -15.1, lz);
            
        } 
    }
    glEnd();

    //glColor3d(1,1, 1);  // 壁の色(白)
    //glColor3d(1, 1, 1);  // 壁の色(空色)
    //glBegin(GL_QUADS);
    //glVertex3d(ground_max_x, ground_max_y, -ground_max_z-0.1);
    //glVertex3d(ground_max_x, -ground_max_y, -ground_max_z - 0.1);
    //glVertex3d(-ground_max_x, -ground_max_y, -ground_max_z - 0.1);
    //glVertex3d(-ground_max_x, ground_max_y, -ground_max_z - 0.1);
    //glEnd();

    glRasterPos3i(-ground_max_x-0.99, -15, -ground_max_z);
    glDrawPixels(640, 400, GL_RGB, GL_UNSIGNED_BYTE, &image[0][0][0]);
    glRasterPos3i(0, -15, -ground_max_z);
    glDrawPixels(640, 400, GL_RGB, GL_UNSIGNED_BYTE, &image[0][0][0]);
    

    glColor3d(0, 0, 0);  // 大地の線の色
    glBegin(GL_LINES);
    /*地面の罫線*/
    for (double lz = -ground_max_z; lz <= ground_max_z; lz += 5) {
        glVertex3d(-ground_max_x,-15,lz);
        glVertex3d(ground_max_x,-15,lz);
        
    }
    for (double lx = -ground_max_x; lx <= ground_max_x; lx += 5) {
        glVertex3d(lx,-15, ground_max_z);
        glVertex3d(lx,-15, -ground_max_z);
    }
    /*奥の壁の線*/
    //for (double ly = -15; ly <= ground_max_y*2; ly += 5.0) {
    //    glVertex3d(-ground_max_x, ly, -ground_max_z);
    //    glVertex3d(ground_max_x, ly, -ground_max_z);
    //}
    //for (double lx = -ground_max_x; lx <= ground_max_x; lx += 5.0) {
    //    glVertex3d(lx, -15, -ground_max_z);
    //    glVertex3d(lx, ground_max_y*2, -ground_max_z);
    //}
    
    glEnd();
    
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

    
    /* 地面のひょうじ　*/
    Ground();

    GLfloat color[4] = { 0,0,0,1 };
    GLdouble normal[3] = { 0,0,0 };

    Vector tmpNormal;

    glPointSize(10);
    glBegin(GL_POINTS);
    double dist = nearestPoint.distanceTo(pointer[0]);
    double c = -dist *(0.1)  + 1;
    if (deformFlag != -1) {
        glColor3f(c,0 , 1);
        glVertex3f(pointer[0].x, pointer[0].y, pointer[0].z);
        glVertex3f(pointer[1].x, pointer[1].y, pointer[1].z);
    }
    else if (rotateFlag != -1) {
        glColor3f(0, 1, 1);
        glVertex3f(pointer[0].x, pointer[0].y, pointer[0].z);
        glVertex3f(pointer[1].x, pointer[1].y, pointer[1].z);
    }
    else if (scaleFlag != -1) {
        glColor3f(1, 1, 1);
        glVertex3f(pointer[0].x, pointer[0].y, pointer[0].z);
        glVertex3f(pointer[1].x, pointer[1].y, pointer[1].z);
    }
    else {
        glColor3f(0, 0, 0);
        glVertex3f(pointer[0].x, pointer[0].y, pointer[0].z);
        glVertex3f(pointer[1].x, pointer[1].y, pointer[1].z);
    }
    
    
    glEnd();

    if (dist < 10) {
        glColor3f(1, 0, 0);
        glPointSize(20);
        glBegin(GL_POINTS);
        glVertex3f(nearestPoint.x, nearestPoint.y, nearestPoint.z);
        glEnd();
    }
    
    
    
   
    /* ポリゴンの描画  */

    switch (shape)
    {
    case ball:
        glBegin(GL_QUADS);
        
        for (int i = 1; i < latitudeNUM-1; i++) {

            for (int j = 0; j < longitudeNUM; j++) {
                int k = i + j * 2;
                if ( k % 5 == 0) {
                    glColor3f(0.5,1, 0.1);
                }
                else if(k % 5 == 1) {
                    glColor3f(0.5, 0.9, 0.2);
                }
                else if (k % 5 == 2) {
                    glColor3f(0.5, 0.80, 0.3);
                }
                else if (k % 5 == 3) {
                    glColor3f(0.5, 0.7, 0.4);
                }
                else if (k % 5 == 4) {
                    glColor3f(0.5, 0.6, 0.5);
                }
                else {
                    glColor3f(0, 0, 0);
                }

                glVertex3f(point[i][j].x, point[i][j].y, point[i][j].z);
                glVertex3f(point[i + 1][j].x, point[i + 1][j].y, point[i + 1][j].z);
                glVertex3f(point[i + 1][(j + 1) % longitudeNUM].x, point[i + 1][(j + 1) % longitudeNUM].y, point[i + 1][(j + 1) % longitudeNUM].z);
                glVertex3f(point[i][(j + 1) % longitudeNUM].x, point[i][(j + 1) % longitudeNUM].y, point[i][(j + 1) % longitudeNUM].z);
            }
        }
        
        for (int j = 0; j < longitudeNUM; j++) {
            int k = j * 2;
            if (k % 5 == 0) {
                glColor3f(0.5, 1, 0.1);
            }
            else if (k % 5 == 1) {
                glColor3f(0.5, 0.9, 0.2);
            }
            else if (k % 5 == 2) {
                glColor3f(0.5, 0.80, 0.3);
            }
            else if (k % 5 == 3) {
                glColor3f(0.5, 0.7, 0.4);
            }
            else if (k % 5 == 4) {
                glColor3f(0.5, 0.6, 0.5);
            }
            else {
                glColor3f(0, 0, 0);
            }

            glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
            glVertex3f(point[1][j].x, point[1][j].y, point[1][j].z);
            glVertex3f(point[1][(j + 1) % longitudeNUM].x, point[1][(j + 1) % longitudeNUM].y, point[1][(j + 1) % longitudeNUM].z);
            glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
            
            glVertex3f(point[latitudeNUM][0].x, point[latitudeNUM][0].y, point[latitudeNUM][0].z);
            glVertex3f(point[latitudeNUM][0].x, point[latitudeNUM][0].y, point[latitudeNUM][0].z);
            glVertex3f(point[latitudeNUM-1][j].x, point[latitudeNUM-1][j].y, point[latitudeNUM-1][j].z);
            glVertex3f(point[latitudeNUM - 1][(j + 1) % longitudeNUM].x, point[latitudeNUM - 1][(j + 1) % longitudeNUM].y, point[latitudeNUM - 1][(j + 1) % longitudeNUM].z);
            
            
        }

        glEnd();
        break;
    case cube:
        

        glBegin(GL_QUADS);
        
        /*　上面　*/
        glColor3f(0, 0, 1);
        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);
        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);
        
        /*　下面　*/
        glColor3f(0, 1, 1);
        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);
        
        /*　正面　*/
        glColor3f(1, 0.5, 0.5);
        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);
        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);

        /*　背面　*/
        glColor3f(1, 1, 0);
        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);

        /*　右側面　*/
        glColor3f(1, 0, 0);
        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);
        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);
        /*　左側面　*/
        glColor3f(0, 1, 0);
        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);
        
        glEnd();
        break;

    case hexagon:
        glPointSize(20);
        glColor3f(0.1, 0.8, 0.1);
        glBegin(GL_POINTS);
        glColor3f(0.6, 1.0, 0.8);        
        for (int i = 0; i < 6; i++) {
            glVertex3f(point[0][i].x, point[0][i].y, point[0][i].z);
        }
        glEnd();

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 6; i++) {
            glVertex3f(point[0][i].x, point[0][i].y, point[0][i].z);
        }
        glEnd();
        break;
    case shadowCube:
        //陰影ON-----------------------------
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);//光源0を利用
        //-----------------------------------
        //色
        color[0] = 1;
        color[1] = 1;
        color[2] = 1;
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
        glBegin(GL_QUADS);
        
        /*　上面　*/
        //垂線の向き
        tmpNormal = point[0][0] - point[1][0];
        normal[0] = tmpNormal.x;
        normal[1] = tmpNormal.y;
        normal[2] = tmpNormal.z;
        glNormal3dv(normal);

        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);
        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);

        /*　下面　*/
        //垂線の向き
        tmpNormal = point[1][0] - point[0][0];
        normal[0] = tmpNormal.x;
        normal[1] = tmpNormal.y;
        normal[2] = tmpNormal.z;
        glNormal3dv(normal);

        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);

        /*　正面　*/
        //垂線の向き
        tmpNormal = point[0][0] - point[0][1];
        normal[0] = tmpNormal.x;
        normal[1] = tmpNormal.y;
        normal[2] = tmpNormal.z;
        glNormal3dv(normal);
        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);
        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);

        /*　背面　*/
        tmpNormal = point[0][1] - point[0][0];
        normal[0] = tmpNormal.x;
        normal[1] = tmpNormal.y;
        normal[2] = tmpNormal.z;
        glNormal3dv(normal);

        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);

        /*　右側面　*/
        tmpNormal = point[0][0] - point[0][2];
        normal[0] = tmpNormal.x;
        normal[1] = tmpNormal.y;
        normal[2] = tmpNormal.z;
        glNormal3dv(normal);

        glVertex3f(point[0][0].x, point[0][0].y, point[0][0].z);
        glVertex3f(point[0][1].x, point[0][1].y, point[0][1].z);
        glVertex3f(point[1][1].x, point[1][1].y, point[1][1].z);
        glVertex3f(point[1][0].x, point[1][0].y, point[1][0].z);
        /*　左側面　*/
        tmpNormal = point[0][2] - point[0][0];
        normal[0] = tmpNormal.x;
        normal[1] = tmpNormal.y;
        normal[2] = tmpNormal.z;
        glNormal3dv(normal);

        glVertex3f(point[0][2].x, point[0][2].y, point[0][2].z);
        glVertex3f(point[0][3].x, point[0][3].y, point[0][3].z);
        glVertex3f(point[1][3].x, point[1][3].y, point[1][3].z);
        glVertex3f(point[1][2].x, point[1][2].y, point[1][2].z);
        glEnd();

        //陰影OFF-----------------------------
        glDisable(GL_LIGHTING);
        //-----------------------------------
        break;
    default:
        break;
    }

    



    


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
    glutTimerFunc(15, timer, 0);

    /* オブジェクトの回転角度を1.0度ずつ増加させる */
    //BoxRotate += 1;
    
    
    if (BoxRotate > 360.0) BoxRotate -= 360.0;

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
        }
        break;
    case 'i':
        shokika();      /* 初期化 */
        break;
    case 's':
        kirikae(1);      /* 切替 */
        break;
    case 'q':
        exit(0);      /* プログラム終了 */
        break;
    }
    Sleep(800);

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
            shokika();      /* 初期化 */
            break;

        case GLUT_MIDDLE_BUTTON:/* マウス中ボタンを押した時の処理 */
            break;

        case GLUT_RIGHT_BUTTON: /* マウス右ボタンを押した時の処理 */
            PressButton = button;
            kirikae(1);      /* 切替 */
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
    int winWidth = 800;
    int winHeight = 600;
    /* ウインドウの縦横の比を計算 */
    float aspect = (float)winWidth / (float)winHeight;

    /* OpenGLウインドウ作成までの初期化 */
    glutInitWindowPosition(0, 0);                   /* ウインドウ表示位置 */
    glutInitWindowSize(winWidth, winHeight);        /* ウインドウサイズ */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);   /* 描画モード */
    glutCreateWindow(windowTitle);                  /* ウインドウの表示 */
    glClearColor(0, 0, 0, 1.0);              /* 画面消去色の設定 */

    

    //光源の設定--------------------------------------
    GLfloat light_position0[] = { -10, 0, 10, 1.0 }; //光源0の座標
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0); //光源0を

    /* イベント発生時に呼び出す関数の登録 */
    glutKeyboardFunc(keyboard);     /* キーボードを押した時 */
    glutMouseFunc(mouseButton);  /* マウスボタンを押した時*/
    glutMotionFunc(mouseDrag);    /* マウスドラッグした時 */
    glutDisplayFunc(display);      /* 画面表示 */
    glutTimerFunc(15, timer, 0);    /* タイマーを15ミリ秒後に設定 */

    /* CG描画設定 */
    glMatrixMode(GL_PROJECTION);    /* 透視投影(遠近投影法)設定モードに切り替え */
    glLoadIdentity();               /* 透視投影行列を初期化 */
    gluPerspective(50.0, aspect, 1.0, 100.0);    /* 透視投影行列の設定 */
      /* 視野角45度, 縦横比 aspect，描画前面までの奥行 1.0，描画背面までの奥行 20.0 */

    

    glEnable(GL_DEPTH_TEST);        /* 隠面消去を有効にする */

    /* 画像の読込 */
    ppm_read("universe.ppm", &image[0][0][0]);
}

