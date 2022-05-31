/* ヘッダファイル */
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "Leap.h"
#include "SampleListener.h"
#include "freeglut.h"

using namespace std;
using namespace Leap;

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
    controller.addListener(listener);

    /* 初期化 */
    glutInit(&argc, argv);  /* OpenGL の初期化 */
    myInit(argv[0]);        /* ウインドウ表示と描画設定の初期化 */

    /* イベント処理ループ */
    glutMainLoop();

    /* プログラム終了 */
    return(0);
}

