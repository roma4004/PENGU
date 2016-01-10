#pragma once
using namespace sf;

void CreateRandWorld();

void setObj(float x, float y);

void setBody(float x, float y, String type);

void setCamCenter(float coordX, float coordY);
								 
void menu(RenderWindow &window);

void eventsOn();

void DrawText(int fontSize, float posX, float posY, String setText,
	Color colorOfText = Color::Black, String FontFamily = "times");