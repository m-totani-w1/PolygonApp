#pragma once
/* 関数のプロトタイプ宣言 */
void display(void);
void timer(int timerID);
void keyboard(unsigned char key, int x, int y);
void mouseButton(int button, int state, int x, int y);
void mouseDrag(int x, int y);
void InitPoint();
void myInit(char* windowTitle);



