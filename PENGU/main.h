#pragma once
using namespace sf;

void setObj(float x, float y);

void setBody(float x, float y, long type);	

void menu(RenderWindow &window);

void DrawText(int fontSize, float posX, float posY, String setText,
	Color colorOfText = Color::Black, String FontFamily = "times");