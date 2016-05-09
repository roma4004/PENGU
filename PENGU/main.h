#pragma once

void CreateRandWorld();

void setObj(float x, float y, void *type);

void setBody(float x, float y, void *type);

void setCamCenter(float coordX, float coordY);
								 
void menu(RenderWindow &window);

void eventsOn();

void setZoomRate(float W, float H, int wheelDelta);

void drawSprite(Sprite targetSprite, b2Vec2 pos, float angle);

float drawtxt = 0, drawtxt2 = 0, drawtxt3 = 0,
	  winSizeX = 0, winSizeY = 0, zoomRate = 0, zoomfactor = 1;  // drawtxt - ���������� ����������; winSize - ������� ���� � �������� ����, use in autoResize
int zoomCnt = 0, maxZoom = 10, minZoom = -20;  //zoomCnt - ������� ������� �����������/���������, ������������(maxZoom) � �����������(minZoom) ������� �����������
unsigned char optimaFPS = 60; 