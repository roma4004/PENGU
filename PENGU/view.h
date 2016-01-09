#pragma once
using namespace sf;
View view;
void setCamCenter(float coordX, float coordY) {
	view.setCenter(coordX, coordY);
	}
void viewMove() {
	Event onFocusEvent;	//тут еще не доделан фокус, нужно как в том управлении игроком сделать
	while (window.pollEvent(onFocusEvent)) {
	if (onFocusEvent.type == Event::GainedFocus){		
		if (Keyboard::isKeyPressed(Keyboard::D)) view.move( 8.f,  0.f);
		if (Keyboard::isKeyPressed(Keyboard::A)) view.move(-8.f,  0.f);
		if (Keyboard::isKeyPressed(Keyboard::W)) view.move( 0.f, -8.f);
		if (Keyboard::isKeyPressed(Keyboard::S)) view.move( 0.f,  8.f);	 
		}
	}
}