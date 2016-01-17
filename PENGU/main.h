#pragma once
using namespace sf;

void CreateRandWorld();

void setObj(float x, float y, String type);

void setBody(float x, float y, String type);

void setCamCenter(float coordX, float coordY);
								 
void menu(RenderWindow &window);

void eventsOn();

void setZoomRate(float W, float H, int wheelDelta);

float drawtxt = 0, drawtxt2 = 0;                         // drawtxt - ���������� ����������
int zoomCnt = 0, maxZoomTop = 10, maxZoomBottom = -20;  //zoomCnt - ������� ������� �����������/���������, ������������(maxZoomTop) � �����������(maxZoomBottom) ������� �����������
float winSizeX = 0, winSizeY = 0;	               	   // ������� ���� � �������� ����, use in autoResize
unsigned char optimaFPS = 60;			 