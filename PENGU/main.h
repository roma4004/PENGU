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
	  winSizeX = 0, winSizeY = 0, zoomRate = 0;  // drawtxt - отладочна€ информаци€; winSize - –азмеры окна с прошлого раза, use in autoResize
int zoomCnt = 0, maxZoom = 10, minZoom = -20;  //zoomCnt - текуща€ позици€ приблежени€/отдалени€, максимальна€(maxZoomTop) и минимальна€(maxZoomBottom) позици€ приближени€
unsigned char optimaFPS = 60; 