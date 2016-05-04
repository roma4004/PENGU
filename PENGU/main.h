#pragma once

void CreateRandWorld();

void setObj(float x, float y, void *type);

void setBody(float x, float y, void *type);

void setCamCenter(float coordX, float coordY);
								 
void menu(RenderWindow &window);

void eventsOn();

void setZoomRate(float W, float H, int wheelDelta);

void drawSprite(Sprite targetSprite, b2Vec2 pos, float angle);

float drawtxt = 0, drawtxt2 = 0, winSizeX = 0, winSizeY = 0;  // drawtxt - ���������� ����������; winSize - ������� ���� � �������� ����, use in autoResize
int zoomCnt = 0, maxZoomTop = 10, maxZoomBottom = -20;  //zoomCnt - ������� ������� �����������/���������, ������������(maxZoomTop) � �����������(maxZoomBottom) ������� �����������
unsigned char optimaFPS = 60;			 