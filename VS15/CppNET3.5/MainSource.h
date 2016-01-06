#pragma once

using namespace sf;

void DrawText(int fontSize, float posX, float posY, String setText,
	Color colorOfText = Color::Black, String FontFamily = "times");

RenderWindow window(VideoMode(200, 200), "SFML works!");