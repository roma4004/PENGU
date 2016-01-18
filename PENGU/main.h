#pragma once

void CreateRandWorld();

void setObj(float x, float y, String type);

void setBody(float x, float y, String type);

void setCamCenter(float coordX, float coordY);
								 
void menu(RenderWindow &window);

void eventsOn();

void setZoomRate(float W, float H, int wheelDelta);

float drawtxt = 0, drawtxt2 = 0;                         // drawtxt - отладочна€ информаци€
int zoomCnt = 0, maxZoomTop = 10, maxZoomBottom = -20;  //zoomCnt - текуща€ позици€ приблежени€/отдалени€, максимальна€(maxZoomTop) и минимальна€(maxZoomBottom) позици€ приближени€
float winSizeX = 0, winSizeY = 0;	               	   // –азмеры окна с прошлого раза, use in autoResize
unsigned char optimaFPS = 60;			 