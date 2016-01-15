#pragma once
using namespace sf;

void CreateRandWorld();

void setObj(float x, float y, String type);

void setBody(float x, float y, String type);

void setCamCenter(float coordX, float coordY);
								 
void menu(RenderWindow &window);

void eventsOn();

void setZoomRate(float W, float H, int wheelDelta);
