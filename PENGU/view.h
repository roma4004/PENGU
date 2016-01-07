#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
View view;
void getplayercoordinateforview(float x, float y) { 
	view.setCenter(x, y); 
	}
void viewMove() {
	if (Keyboard::isKeyPressed(Keyboard::D))		view.move(8,0);
	if (Keyboard::isKeyPressed(Keyboard::A))		view.move(-8, 0);
	if (Keyboard::isKeyPressed(Keyboard::W))		view.move(0, -8);
	if (Keyboard::isKeyPressed(Keyboard::S))		view.move(0, 8);
}