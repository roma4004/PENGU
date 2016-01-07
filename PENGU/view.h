#pragma once
using namespace sf;
View view;
void getplayercoordinateforview(float x, float y) { 
	view.setCenter(x, y); 
	}
void viewMove() {
	if (Keyboard::isKeyPressed(Keyboard::D)) view.move( 8.f,  0.f);
	if (Keyboard::isKeyPressed(Keyboard::A)) view.move(-8.f,  0.f);
	if (Keyboard::isKeyPressed(Keyboard::W)) view.move( 0.f, -8.f);
	if (Keyboard::isKeyPressed(Keyboard::S)) view.move( 0.f,  8.f);
}